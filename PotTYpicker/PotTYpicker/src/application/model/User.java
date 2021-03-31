package application.model;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

/**
 * This class describes a user which can interact with the
 * application PotTYpicker.
 * @author Lee Reese (odq641)
 *
 */
public class User {
	String username;
	String password;
	
	/**
	 * Creates a new instance of User given a
	 * username and password
	 * @param username
	 * @param password
	 */
	public User(String username, String password) {
		this.setUsername(username);
		this.setPassword(password);
	}
	
	/**
	 * Creates a new user given a desired username and password.
	 * Then, writes that users credentials to users.csv.
	 * @param username the new user's username
	 * @param password the new user's password
	 * @return a new user if username is unique, null otherwise
	 * @throws IOException
	 */
	public static User newUser(String username, String password) throws IOException {
		/* Create user and ensure uniqueness */
		if (!unique(username))
			return null;
		
		File userFile = new File("data/users.csv");
		FileWriter userWriter = new FileWriter(userFile, true);
		userWriter.append("\n" + username + "," + password);
		userWriter.close();
		
		User newUser = new User(username, password);
		return newUser;
	}

	/**
	 * Checks users.csv to determine if this user's credentials
	 * are legitimate
	 * @return True if this user exists, false otherwise
	 * @throws IOException If users.csv cannot be found
	 */
	public boolean validate() throws IOException {
		File userFile = new File("data/users.csv");
		Scanner userScan = new Scanner(userFile);
		while(userScan.hasNextLine()) {
			String line = userScan.nextLine();
			String[] tokens = line.split(",");
			if (tokens.length < 2) {
				continue;
			}
			if (this.getUsername().equals(tokens[0]) && this.getPassword().equals(tokens[1])) {
				userScan.close();
				return true;
			}
		}
		userScan.close();
		return false;
	}
	
	/**
	 * This function determines if a username is already
	 * present in the users.csv file
	 * @param username THe username to check
	 * @return True if the username is not in users.csv, false otherwise
	 * @throws IOException If users.csv cannot be found
	 */
	private static boolean unique(String username) throws IOException {
		File userFile = new File("data/users.csv");
		Scanner userScan = new Scanner(userFile);
		while(userScan.hasNextLine()) {
			String line = userScan.nextLine();
			String[] tokens = line.split(",");
			if (tokens.length < 2) {
				continue;
			}
			if (username.equals(tokens[0])) {
				userScan.close();
				return false;
			}
		}
		userScan.close();
		return true;
	}
	
	/**
	 * toString which does nothing more than getUsername. Pretty cool, huh?
	 */
	public String toString() {
		return this.getUsername();
	}
	
	/**
	 * Get this user's username
	 * @return The username
	 */
	public String getUsername() {
		return username;
	}

	/**
	 * Set this user's username
	 * @param username The desired username
	 */
	public void setUsername(String username) {
		this.username = username;
	}

	/**
	 * Get this user's password
	 * @return The password
	 */
	public String getPassword() {
		return password;
	}

	/**
	 * Sets this user's password
	 * @param password The desired password
	 */
	public void setPassword(String password) {
		this.password = password;
	}
	
	
}
