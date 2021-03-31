#include "pcap.h"
#include "dumbnet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>

#define PCAP_MAGIC 0xa1b2c3d4
#define CMD "tcp and dst host %s and src host %s"

char confData[1024];
eth_t *e;
intf_t *i;
pcap_t *p;
char ebuf[2048];
struct intf_entry ie;
char myipstr[32], mymacstr[32];
struct addr myip, mymac;
//variable for sqe, ack, and paylod size
int payloadLen;
unsigned int capturedSeqNum;
unsigned int tmpSeqNum;

struct timev {
	unsigned int tv_sec;
	unsigned int tv_usec;
};

//data prefixing each packet
struct my_pkthdr {
	struct timev ts;
	int caplen;
	int len;
};

int readFile(char *filename ) {

	FILE *fp;
	int confSize;
	int loopCount;

	fp = fopen(filename,"r");
	if(fp == NULL)
	{
		printf("Error opening file, make sure it is a valid file in the directory");
	}

	fseek(fp, 0L, SEEK_END);
	confSize = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	for(loopCount = 0; loopCount < confSize; loopCount++) {

		fread(&confData[loopCount], 1, 1, fp);
	}

	fclose(fp);

	return confSize;

}

void tokenize(char **name, char **vip, char **vmac, char **vport, char **aip, char **amac, char **aport, char **vipnew, char **vmacnew, char **vportnew, char **aipnew, char **amacnew, char **aportnew, char **cinterface, char **timing) {
	
	*name  = strtok(confData, "\n");
	*vip = strtok(NULL, "\n");
	*vmac = strtok(NULL, "\n");
	*vport = strtok(NULL, "\n");
	*aip = strtok(NULL, "\n");
	*amac = strtok(NULL, "\n");
	*aport = strtok(NULL, "\n");
	*vipnew = strtok(NULL, "\n");
	*vmacnew = strtok(NULL, "\n");
	*vportnew = strtok(NULL, "\n");
	*aipnew = strtok(NULL, "\n");
	*amacnew = strtok(NULL, "\n");
	*aportnew = strtok(NULL, "\n");
	*cinterface = strtok(NULL, "\n");
	*timing = strtok(NULL, "\n");

	return;
}

void rmnl(char *s) {
	while( *s != '\n' && *s != '\0' )
		s++;
	*s = '\0';
}

//extracts data from captured packet
void capturedPacket(u_char *user, struct my_pkthdr *captured, u_char *capturedPkt){

//create ptrs to header values
struct eth_hdr *tmpeth;
struct ip_hdr *tmpip;
struct tcp_hdr *tmptcp;
int size_ip;
int size_tcp;
int size_payload;
const char *payload;

tmpeth = (struct eth_hdr*)capturedPkt;
tmpip = (struct ip_hdr*)(capturedPkt + ETH_HDR_LEN);
size_ip = tmpip->ip_hl *4;
tmptcp = (struct tcp_hdr*)(capturedPkt + ETH_HDR_LEN + size_ip );
size_tcp = tmptcp->th_off * 4;
payload = (u_char *)(capturedPkt + ETH_HDR_LEN + size_ip + size_tcp);
size_payload = ntohs(tmpip->ip_len) - (size_ip + size_tcp);
payloadLen = size_payload;
//extract the sequence  number and put it in global variable called capturedSeqNum
capturedSeqNum = ntohl(tmptcp->th_seq);

return;

}



int main( int argc, char * argv[] ) {

	int readFD; //file descriptor for pcap file	
	struct pcap_file_header fileHeader;  //header structure for pcap header
	int readFileRet; 
	int packNum = 0; //count of packet that you are on
	int readPackRet;
	struct my_pkthdr packHeader; // header structure for packet
	//variables that help compute relatives times of packets
	int firsttime = 1;
	unsigned int b_sec; 
	int b_usec;
	unsigned int c_sec;
	int c_usec;
	//header structures from libdnet, eventually fileld with corresponding data from pcap file
	struct eth_hdr *ethHeader;
	struct ip_hdr *ipHeader;
	struct arp_hdr *arpHeader;
	struct tcp_hdr *tcpHeader;
	struct udp_hdr *udpHeader;
	struct icmp_hdr *icmpHeader;
	char *packet;
	struct addr srcad, srcha, srcad2, srcha2;
	int readTypeRet;
	//Variables that contain respective conf file data:
	char *confName; //configuration file name passed in to readFile() as a parameter
	char *pcapName; //pcapfile name
	char *vip; // victim ip
  	char *vmac; // victim mac
	char *vport; // victim port
	char *aip; // attacker ip
	char *amac; // attacker mac
	char *aport; // attacker port
	char *vipnew; // desired victim ip
	char *vmacnew; // desired victim mac 
	char *vportnew; // desired victim port number
	char *aipnew; // desired attacker ip
	char *amacnew; // desired attacker mac 
	char *aportnew; // desired attacker port
	char *cinterface; //  newtork interface
	char *timing; // timing option for when packet is sent
	int confFileLen; //length of configuration file, retruned from readFile()
	struct addr confnewvip, confnewvmac, confnewaip, confnewamac; // addr structs that will hold new victim and attacker data
	unsigned short confnewaport, confnewvport, confaport, confvport; //will be used to store the unsigned long representatiopn of the port #s read in from configuration file
	int sendFlag = 0;
	int receiveFlag = 0;
	unsigned short tmpaportnew, tmpvportnew;
	//variables used when setting bpf filter
	char cmd[128];
	struct bpf_program fcode;
	uint32_t localnet, netmask;

	if(argc < 2 || argc > 3)
	{
		printf("\nUsage: ./assign4 -s(optional send switch) <configuration file name>\n");
		exit(1);
	}

	//open configuration file successfully and read data from file
	if(argc == 2) // if s switch is not on
  	{
		confName = argv[1];
		confFileLen = readFile(confName);
		tokenize(&pcapName, &vip, &vmac, &vport, &aip, &amac, &aport, &vipnew, &vmacnew, &vportnew, &aipnew, &amacnew, &aportnew, &cinterface, &timing);
		//convert ascii data to network form and store it in respective addr struct
		addr_aton(vipnew, &confnewvip);
		addr_aton(vmacnew, &confnewvmac);
		addr_aton(aipnew, &confnewaip);
		addr_aton(amacnew, &confnewamac);
  	}
    	else if(argc == 3) //if s switch is on
  	{
    		confName = argv[2];
		confFileLen = readFile(confName);
   		tokenize(&pcapName, &vip, &vmac, &vport, &aip, &amac, &aport, &vipnew, &vmacnew, &vportnew, &aipnew, &amacnew, &aportnew, &cinterface, &timing);
   		addr_aton(vipnew, &confnewvip);
      		addr_aton(vmacnew, &confnewvmac);
		addr_aton(aipnew, &confnewaip);
		addr_aton(amacnew, &confnewamac);
		
  	}

	//opening pcap file and then doing an error check 
	readFD = open(pcapName, O_RDWR); 
	if(readFD < 0)
	{
		printf("\nError reading file, make sure it exists\n");
		exit(1);
	}

	readFileRet = read(readFD, &fileHeader, sizeof(fileHeader));
	if(readFileRet < 0)
	{
		printf("\nError: not able to read!\n");
	}
	
	if(fileHeader.magic == PCAP_MAGIC)
	{
		printf("\n\tPCAP_MAGIC\n");
		printf("\tVersion major number = %hu\n", fileHeader.version_major);
		printf("\tVersion minor number = %hu\n", fileHeader.version_minor);
		printf("\tGMT to local correction = %u\n", fileHeader.thiszone);
		printf("\tTimestamp accuracy = %u\n", fileHeader.sigfigs);
		printf("\tSnaplen = %u\n", fileHeader.snaplen);
		printf("\tLinktype = %u\n", fileHeader.linktype);
	}
 	
	rmnl(cinterface);
	if(argc == 3) {
		//opening device before while loop
  		i = intf_open();
		strncpy(ie.intf_name, cinterface, 60);
		if( intf_get(i, &ie) == -1 )
		{
			printf("\n\nError with intf\n\n");
			exit(-1);
		}
		mymac = ie.intf_link_addr;
		if( addr_ntop(&mymac, mymacstr, 32) == NULL ) 
		{
			exit(-1);
		}
		myip = ie.intf_addr;
		if( addr_ntop(&myip, myipstr, 32) == NULL )
		{
			exit(-1);
		}
		e = eth_open(cinterface);
		if(e == NULL) 
		{
			printf("\n\nInterface did not open\n\n");
			exit(-1);
		}
		p = pcap_open_live(cinterface, 65535, 1, 1000, ebuf);
		if(p == NULL)
		{
			printf("\n\nError with pcap_open_live\n\n");
			exit(-1);
		}
		/*p = pcap_create(cinterface, ebuf); //Alternate solution to pcapopen live look these up later
  		pcap_set_timeout(p, 500);
  		pcap_set_immediate_mode(p, 1);
  		pcap_activate(p);*/
		//setting filter so that only packets that are coming to my machine will be accepted
		if( pcap_lookupnet(cinterface, &localnet, &netmask, ebuf) < 0 )
		{
			printf("\n\nError with pcap_lookupnet\n\n");
			exit(-1);
		}
		snprintf(cmd, sizeof(cmd), CMD, aipnew, vipnew);
		printf("\n\nFilter: %s\n\n", cmd);
		if( pcap_compile(p, &fcode, cmd, 0, netmask) < 0 ) 
		{
			printf("\n\nError with pcap_compile\n\n");
			exit(-1);
		}
		if( pcap_setfilter(p, &fcode) < 0 ) 
		{
			printf("\n\nError with pcap_setfilter\n\n");
			exit(-1);
		}
	}

  
	while( readPackRet = read(readFD, &packHeader, sizeof(packHeader)) ) {
			
		printf("\n\tPacket %d\n", packNum);
		//computing relative time of each packet, if first packet(packet 0) then its time is 0
		if( firsttime ) 
		{
			firsttime = 0;
			b_sec = packHeader.ts.tv_sec;
			b_usec = packHeader.ts.tv_usec;
		}
		
		c_sec = packHeader.ts.tv_sec - b_sec;
		c_usec = packHeader.ts.tv_usec - b_usec;
		while( c_usec < 0 ) {
			c_usec += 1000000;
			c_sec--;
		}
		printf("\t%05u.%06u\n", (unsigned)c_sec, (unsigned)c_usec);
		printf("\tCaptured Packet Length = %d\n", packHeader.caplen);
		printf("\tActual Packet Length = %d\n", packHeader.len);
		packNum++;
		packet = (char *)malloc(packHeader.caplen);
		if(packet == NULL) 
		{
			printf("Malloc Failed!");
			exit(1);
		}
		readTypeRet = read(readFD, packet, packHeader.caplen);
		//error check right here when you have time
		ethHeader = (struct eth_hdr *)packet;
		ipHeader = (struct ip_hdr *)(packet + ETH_HDR_LEN);
		arpHeader = (struct arp_hdr *)(packet + ETH_HDR_LEN);
		addr_pack(&srcha, ADDR_TYPE_ETH, ETH_ADDR_BITS, &(ethHeader->eth_src), ETH_ADDR_LEN);
		addr_pack(&srcad, ADDR_TYPE_ETH, ETH_ADDR_BITS, &(ethHeader->eth_dst), ETH_ADDR_LEN);
		printf("\tEthernet Header\n");
		printf("\t\teth_src = %s\n", addr_ntoa(&srcha));
		printf("\t\teth_dst = %s\n", addr_ntoa(&srcad));
		// if ethernet values match the ones from the conf file, you can enter this statement and replace packet etehrnet src and dst values, statement can be used to skip unknowm(non matching) packets
		if(  ( strcmp(addr_ntoa(&srcha), amac) == 0 || strcmp(addr_ntoa(&srcad), amac) == 0 ) && ( strcmp(addr_ntoa(&srcha), vmac) == 0 || strcmp(addr_ntoa(&srcad), vmac) == 0 ) ) 
		{
			//if ethernet source mac matches the attacker mac and ethernet dst matches victim mac, so attacker----->>>>>victim, packets being sent out
      			if( strcmp( addr_ntoa(&srcha), amac ) == 0 && strcmp( addr_ntoa(&srcad), vmac ) == 0 )
			{
				sendFlag = 0;
				memcpy(&ethHeader->eth_src, &confnewamac.addr_eth, ETH_ADDR_LEN); //replace mac src address with replay attacker mac address from conf file
				memcpy(&ethHeader->eth_dst, &confnewvmac.addr_eth, ETH_ADDR_LEN); //replace mac dst address with replay victim mac address from conf file
				sendFlag = 1;
			}
			else // victim------>>>>>attacker, packets coming in
			{
				receiveFlag = 0;
				memcpy(&ethHeader->eth_src, &confnewvmac.addr_eth, ETH_ADDR_LEN); //replace mac src address with replay victim mac address from conf file
				memcpy(&ethHeader->eth_dst, &confnewamac.addr_eth, ETH_ADDR_LEN); //replace mac dst address with replay attacker mac address from conf file
				receiveFlag = 1;
      			}
			printf("\t\trep_src = %s\n", eth_ntoa(&ethHeader->eth_src) );
			printf("\t\trep_dst = %s\n", eth_ntoa(&ethHeader->eth_dst) );
		}
		if( ntohs(ethHeader->eth_type) == ETH_TYPE_IP) 
		{	

			addr_pack(&srcha2, ADDR_TYPE_IP, IP_ADDR_BITS, &(ipHeader->ip_src), IP_ADDR_LEN);
			addr_pack(&srcad2, ADDR_TYPE_IP, IP_ADDR_BITS, &(ipHeader->ip_dst), IP_ADDR_LEN);
			printf("\t\t\tIP\n");
			printf("\t\t\t\tip len = %hu\n", ntohs(ipHeader->ip_len));
			printf("\t\t\t\tip src = %s\n", addr_ntoa(&srcha2));
			printf("\t\t\t\tip dst = %s\n", addr_ntoa(&srcad2));
      			// if ip header values match the ones from the conf file, you can enter this statement and replace the ip src and dst values
			if(  ( strcmp(addr_ntoa(&srcha2), aip) == 0 || strcmp(addr_ntoa(&srcad2), aip) == 0 ) && ( strcmp(addr_ntoa(&srcha2), vip) == 0 || strcmp(addr_ntoa(&srcad2), vip) == 0 ) ) 
			{
				//if  source ip  matches the attacker ip and  dst ip  matches victim ip, so attacker----->>>>>victim, packets being sent out
      				if( strcmp( addr_ntoa(&srcha2), aip ) == 0 && strcmp( addr_ntoa(&srcad2), vip ) == 0 )
				{
					memcpy(&ipHeader->ip_src, &confnewaip.addr_ip, IP_ADDR_LEN); //replace src ip address with replay attacker ip address from conf file
					memcpy(&ipHeader->ip_dst, &confnewvip.addr_ip, IP_ADDR_LEN); //replace dst ip address with replay victim ip address from conf file
				}
				else // victim------>>>>>attacker, packets coming in
				{
					memcpy(&ipHeader->ip_src, &confnewvip.addr_ip, IP_ADDR_LEN); //replace src ip address with replay victim ip address from conf file
					memcpy(&ipHeader->ip_dst, &confnewaip.addr_ip, IP_ADDR_LEN); //replace dst ip address with replay attacker ip address from conf file
      				}
				printf("\t\t\t\trep_src = %s\n", ip_ntoa(&ipHeader->ip_src) );
				printf("\t\t\t\trep_dst = %s\n", ip_ntoa(&ipHeader->ip_dst) );
			}
			if( ipHeader->ip_p == IP_PROTO_TCP )
			{
				tcpHeader = (struct tcp_hdr *)( packet + ETH_HDR_LEN + (ipHeader->ip_hl * 4) );
				//convert port#,s from string to unsigned short
				confvport = (unsigned short) strtoul(vport, NULL, 0);
				confaport = (unsigned short) strtoul(aport, NULL, 0);
				confnewvport = (unsigned short) strtoul(vportnew, NULL, 0);
				confnewaport = (unsigned short) strtoul(aportnew, NULL, 0);
				if( ( ntohs(tcpHeader->th_sport) == confaport || ntohs(tcpHeader->th_dport) == confaport ) && ( ntohs(tcpHeader->th_sport) == confvport || ntohs(tcpHeader->th_dport) == confvport ) )
				{
					if( ntohs(tcpHeader->th_sport) == confaport && ntohs(tcpHeader->th_dport) == confvport )
					{

						tmpaportnew = htons(confnewaport);
						tmpvportnew = htons(confnewvport);
						memcpy(&tcpHeader->th_sport, &tmpaportnew, sizeof(tcpHeader->th_sport)); 
						memcpy(&tcpHeader->th_dport, &tmpvportnew, sizeof(tcpHeader->th_dport));
					}
					else
					{
						tmpaportnew = htons(confnewaport);
						tmpvportnew = htons(confnewvport);
						memcpy(&tcpHeader->th_sport, &tmpvportnew, sizeof(tcpHeader->th_sport));
						memcpy(&tcpHeader->th_dport, &tmpaportnew, sizeof(tcpHeader->th_dport));
					}
					printf("\t\t\t\t\tTCP\n");
					printf("\t\t\t\t\t\tSrc Port = %hu\n", ntohs(tcpHeader->th_sport) ); 
					printf("\t\t\t\t\t\tDst Port = %hu\n", ntohs(tcpHeader->th_dport) );
					printf("\t\t\t\t\t\tSeq = %u\n", ntohl(tcpHeader->th_seq));
					printf("\t\t\t\t\t\tAck = %u\n", ntohl(tcpHeader->th_ack));
				}
		
			}
			else if( ipHeader->ip_p == IP_PROTO_UDP)
			{
				udpHeader = (struct udp_hdr *)( packet + ETH_HDR_LEN + (ipHeader->ip_hl * 4) );
				printf("\t\t\t\t\tUDP\n");
				printf("\t\t\t\t\t\tSrc Port = %hu\n", ntohs(udpHeader->uh_sport) );
				printf("\t\t\t\t\t\tDst Port = %hu\n", ntohs(udpHeader->uh_dport) );

			}
			else if( ipHeader->ip_p == IP_PROTO_ICMP)
			{
				icmpHeader = (struct icmp_hdr *)(packet + ETH_HDR_LEN + (ipHeader->ip_hl*4) );
				printf("\t\t\t\t\tICMP\n");
				if(ICMP_ECHOREPLY == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tEcho Reply\n"); 
				}
    				else if(ICMP_UNREACH == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tDestination Unreachable\n"); 
				}
    				else if(ICMP_SRCQUENCH == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tSource Quench\n"); 
				}
    				else if(ICMP_REDIRECT == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tRoute Redirection\n"); 
				}
    				else if(ICMP_ALTHOSTADDR == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tRoute Redirection\n"); 
				}
    				else if(ICMP_ECHO == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tEcho\n"); 
				}
    				else if(ICMP_RTRADVERT == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tRouter Advertisement\n"); 
				}
    				else if(ICMP_TIMEXCEED == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tTime Exceeded\n"); 
				}
    				else if(ICMP_PARAMPROB == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tBad IP Header\n"); 
				}
    				else if(ICMP_TSTAMP == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tTime Stamp Request\n"); 
				}
    				else if(ICMP_TSTAMPREPLY == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tTime Stamp Reply\n"); 
				}
    				else if(ICMP_INFO == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tInformation Request\n"); 
				}
    				else if(ICMP_INFOREPLY == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tInformation Reply\n"); 
				}
				else if(ICMP_MASK == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tAddress Mask Request\n"); 
				}
    				else if(ICMP_MASKREPLY == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tAddress Mask Reply\n"); 
				}
    				else if(ICMP_TRACEROUTE == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tTraceroute\n"); 
				}
    				else if(ICMP_DATACONVERR == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tData Conversion Error\n"); 
				}
    				else if(ICMP_MOBILE_REDIRECT == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tMobile Host Redirect\n"); 
				}
    				else if(ICMP_IPV6_WHEREAREYOU == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tIPV6 Where are you?\n"); 
				}
    				else if(ICMP_MOBILE_REG == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tMobile Registration Request\n");
				}
    				else if(ICMP_MOBILE_REGREPLY == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tMobile Registration Reply\n"); 
				}
    				else if(ICMP_DNS == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tDomain Name Request\n"); 
				}
    				else if(ICMP_DNSREPLY == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tDomain Name Reply\n"); 
				}
    				else if(ICMP_SKIP == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tSkip\n"); 
				}
    				else if(ICMP_PHOTURIS == icmpHeader->icmp_type) 
				{ 
					printf("\t\t\t\t\t\tPhoturis\n"); 
				}
    				else 
				{ 
					printf("\t\t\t\t\t\tUnknown\n"); 
				}
			}
			else if( ipHeader->ip_p == IP_PROTO_IGMP)
			{
				printf("\t\t\t\t\tIGMP\n");
			}
			else
			{
				printf("\t\t\t\t\tOTHER\n");
			}

			
		}
		else if( ntohs(ethHeader->eth_type) == ETH_TYPE_ARP) 
		{
			
			printf("\t\t\tARP\n");
			if( ntohs(arpHeader->ar_op) == ARP_OP_REQUEST ) 
			{
				printf("\t\t\t\tARP Request\n"); 
			}
			else if( ntohs(arpHeader->ar_op) == ARP_OP_REPLY )
			{
				printf("\t\t\t\tARP Reply\n");
			}
			else if( ntohs(arpHeader->ar_op) == ARP_OP_REVREQUEST )
			{
				printf("\t\t\t\tARP Reverse Request\n");
			}
			else
			{
				printf("\t\t\t\tARP Reverse Reply\n");
			}	


		}
		else
		{
			printf("\t\t\tOTHER\n");
		
		}

		//send packet because -s switch is on and sned flag is on, will only send packet if send value is 1
		if(argc==3 )
		{	

			int n;
			int l;
			int d;
			if(sendFlag==0) {
				continue;
			}
			//if packet is going from us to victim and it is the first packet being sent then send it and then capture packet
			if(sendFlag == 1 && packNum  == 1)
			{

				ip_checksum( (void *)ipHeader, ntohs(ipHeader->ip_len) );
				n = eth_send(e, packet, packHeader.len);
				if(n != packHeader.len) 
				{
					printf("\n\n\t\tPacket not sent\n\n");
				}
				else
				{
					printf("\n\n\t\tPacket sent\n\n");
				}

				if( pcap_loop(p, 1, (pcap_handler)capturedPacket, (u_char *)NULL) < 0 )
				{
					printf("\n\nError with pcap_loop\n\n");
				}

			}
			else if(sendFlag == 1 && packNum == 3) // if packet is being sent out and 2nd packet being sent, caculate new sequence and ack numbers for packet being sent based off packet capture from victim, recalculate the checksum, send the packet , and do another packet capture using pcaploop (handle threeway handshake)
			{ 
				capturedSeqNum += 1;
				tmpSeqNum = htonl(capturedSeqNum);
				memcpy( &tcpHeader->th_ack, &tmpSeqNum, sizeof(tcpHeader->th_ack) );
				ip_checksum( (void *)ipHeader, ntohs(ipHeader->ip_len) );
				l = eth_send(e, packet, packHeader.len);
				if(l != packHeader.len)
				{
					printf("\n\n\t\tPacket Not Sent!\n\n");
				}
				else
				{
					printf("\n\n\t\tPacket Sent\n\n");
				}

				if( pcap_loop(p, 1, (pcap_handler)capturedPacket, (u_char *)NULL) < 0 )
				{
					printf("\n\nError with pcap_loop\n\n");
				}


			}
			else if(sendFlag == 1 && packNum > 3) 
			{
				capturedSeqNum += payloadLen;
				tmpSeqNum = htonl(capturedSeqNum);
				memcpy( &tcpHeader->th_ack, &tmpSeqNum, sizeof(tcpHeader->th_ack) );
				ip_checksum( (void *)ipHeader, ntohs(ipHeader->ip_len) );
				d = eth_send(e, packet, packHeader.len);
				if(d != packHeader.len)
				{
					printf("\n\n\t\tPacket not sent\n\n");
				}
				else
				{
					printf("\n\n\t\tPacket Sent!\n\n");
				}

				if( pcap_loop(p, 1, (pcap_handler)capturedPacket, (u_char*)NULL) < 0 )
				{
					printf("\n\nError with pcap_loop\n\n");
				}
			
			}
			
			
		}
		else
		{
			printf("\n\n\t\tPacket not sent\n\n");
		}


	}
	
	close(readFD);

	return 0;

}



