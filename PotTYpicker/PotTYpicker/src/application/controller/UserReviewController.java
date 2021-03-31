package application.controller;

import java.io.File;
import java.io.IOException;

import application.Main;
import application.model.UserReview;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.ListView;
import javafx.scene.control.TextField;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.AnchorPane;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
/**
 * @author Kevin Nguyen (zpp390)
 *
 * 
 */
public class UserReviewController {
	@FXML
	Button button1,button2,button3,button4,button5,submit; 
	@FXML
	ImageView popup ;  
	@FXML
	ImageView rate1, rate2, rate3, rate4, rate5;
	@FXML
	TextField text_review; 
	@FXML
	Label after_submit; 
	@FXML
	AnchorPane father;
	private int count_click1; 
	private int rating_stars; 

	public void initialize() {
		this.rating_stars = 0;
	}


	public void event1(MouseEvent mouseEvent) {
		File home = new File ("rating");
		if(mouseEvent.getClickCount() % 2 == 0 ){
			for (File file: home.listFiles()){
				if (file.getName().equalsIgnoreCase("empty.png")) {
					double y = rate1.getY(); 
					Image image = new Image (file.toURI().toString());
					rate1.setImage(image);
					this.rating_stars -= 2;
					System.out.println(this.rating_stars);
				}
			}
		}
		
		else {
			if(mouseEvent.getClickCount() % 1 == 0 ){
				for (File file: home.listFiles()){
					if (file.getName().equalsIgnoreCase("gold-star.png")) {
						double y = rate1.getY(); 
						Image image = new Image (file.toURI().toString());
						rate1.setImage(image);
						this.rating_stars++; 
						System.out.println(this.rating_stars);
					}
				}
			}

		}
	}

	public void event2(MouseEvent mouseEvent) {
		File home = new File ("rating");
		if(mouseEvent.getClickCount() % 2 == 0 ){
			for (File file: home.listFiles()){
				if (file.getName().equalsIgnoreCase("empty.png")) {
					Image image = new Image (file.toURI().toString());
					rate2.setImage(image);
					this.rating_stars-= 2;
					System.out.println (this.rating_stars);

				}
			}

		}
		 else {
			if(mouseEvent.getClickCount() % 1 == 0 ){
				for (File file: home.listFiles()){
					if (file.getName().equalsIgnoreCase("gold-star.png")) {
						Image image = new Image (file.toURI().toString());
						rate2.setImage(image);
						this.rating_stars++;
						System.out.println (this.rating_stars);
					}
				}
			}

		}
	}
	
	public void event4(MouseEvent mouseEvent) {
		File home = new File ("rating");
		if(mouseEvent.getClickCount() % 2 == 0 ){
			for (File file: home.listFiles()){
				if (file.getName().equalsIgnoreCase("empty.png")) {
					Image image = new Image (file.toURI().toString());
					rate4.setImage(image);
					this.rating_stars -= 2; 
					System.out.println (this.rating_stars);

				}
			}
		} 
		else {
 
			if(mouseEvent.getClickCount() % 1 == 0 ){
				for (File file: home.listFiles()){
					if (file.getName().equalsIgnoreCase("gold-star.png")) {
						Image image = new Image (file.toURI().toString());
						rate4.setImage(image);
						this.rating_stars++; 
						System.out.println (this.rating_stars);

					}
				}
			}

		}
	}
	public void event3(MouseEvent mouseEvent) {
		File home = new File ("rating");
		if(mouseEvent.getClickCount() % 2 == 0 ){
			for (File file: home.listFiles()){
				if (file.getName().equalsIgnoreCase("empty.png")) {
					Image image = new Image (file.toURI().toString());
					rate3.setImage(image);
					this.rating_stars-= 2;
					System.out.println (this.rating_stars);

				}
			}
		}
		else {
			if(mouseEvent.getClickCount() % 1 == 0 ){
				for (File file: home.listFiles()){
					if (file.getName().equalsIgnoreCase("gold-star.png")) {
						Image image = new Image (file.toURI().toString());
						rate3.setImage(image);
						this.rating_stars++; 
						System.out.println (this.rating_stars);
					}
				}
			}

		}

	}
	
	public void event5(MouseEvent mouseEvent) {

		File home = new File ("rating");
		if(mouseEvent.getClickCount() % 2 == 0 ){
			for (File file: home.listFiles()){
				if (file.getName().equalsIgnoreCase("empty.png")) { 
					Image image = new Image (file.toURI().toString());
					rate5.setImage(image);
					this.rating_stars -= 2; 
					System.out.println (this.rating_stars);
				}
			}
		}
	 else {
			if(mouseEvent.getClickCount() % 1 == 0 ){
				for (File file: home.listFiles()){
					if (file.getName().equalsIgnoreCase("gold-star.png")) {
						Image image = new Image (file.toURI().toString());
						rate5.setImage(image);
						this.rating_stars++; 

						System.out.println (this.rating_stars);

					}
				}
			}

		}
	}

	public void displayOverview(ActionEvent Event) {
		try {
			Parent root = FXMLLoader.load(getClass().getResource("../view/Overview.fxml"));
			Main.stage.setScene(new Scene(root, 800, 800));
			Main.stage.show();
		} catch (IOException e) {
			e.printStackTrace();
		}

	}
	
	public void submit (ActionEvent Event) {
		System.out.println (text_review.getText());
		after_submit.setText("Your review submitted successfully");
		after_submit.setFont(new Font("American Typewriter", 15));
		after_submit.setTextFill(Color.web("#32CD32"));
		
		UserReview newReview = new UserReview(text_review.getText(), LoginController.user.toString(), this.rating_stars, 0);
		newReview.writeReview(SelectionController.restroom);
	}
	public void map_redirect () {
		try {
			Parent root = FXMLLoader.load(getClass().getResource("../view/Map.fxml"));
			Main.stage.setScene(new Scene(root, 800, 800));
			Main.stage.show();

		} catch(Exception e) {
			e.printStackTrace();
		}
	}
	public void send () {
		try {
			Parent root = FXMLLoader.load(getClass().getResource("../view/SendEmail.fxml"));
			Main.stage.setScene(new Scene(root, 800, 800));
			Main.stage.show();

		} catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	private String fitComment() {
		return "test";
	}
}
