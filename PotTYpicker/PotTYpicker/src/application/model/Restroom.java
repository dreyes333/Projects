package application.model;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

public class Restroom {
	private File dir;
	private String roomNumber;
	private ArrayList<UserReview> reviews;

	public Restroom(File file) {
		this.dir = file;
		this.roomNumber = dir.getName();
		this.reviews = new ArrayList<UserReview>();
		loadReviews();
	}
	public String toString() {
		return this.roomNumber;
	}

	public String getRoomNumber() {
		return this.roomNumber;
	}

	public void setName(String roomNumber) {
		this.roomNumber = roomNumber;
	}

	public File getDir() {
		return this.dir;
	}

	public void setDir(File dir) {
		this.dir = dir;
	}



	public ArrayList<UserReview> getReviews() {
		this.reviews.clear();
		this.loadReviews();
		return this.reviews;
	}

	private void loadReviews() {
		for (File file : this.dir.listFiles()) {
			String userName = file.getName().replaceFirst("[.][^.]+$", "");
			try {
				String[] data = readComment(file);
				this.reviews.add(new UserReview(data[2], userName, Integer.parseInt(data[1]), Integer.parseInt(data[0])));
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	private String[] readComment(File review) throws IOException {
		String[] data = {"", "", ""};
		Scanner reviewScan = new Scanner(review);

		/* Get popularity and rating */
		data[0] = reviewScan.nextLine();
		data[1] = reviewScan.nextLine();

		/* Get comment body */
		StringBuilder comment = new StringBuilder();
		while(reviewScan.hasNextLine()) {
			comment.append(reviewScan.nextLine());
		}
		reviewScan.close();
		data[2] = comment.toString();
		return data;
	}
}

