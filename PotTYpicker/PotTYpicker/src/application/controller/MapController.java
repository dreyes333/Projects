package application.controller;

import java.io.IOException;
import java.net.URL;
import java.util.ResourceBundle;

import application.Main;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.StackPane;
import javafx.scene.web.WebEngine;
import javafx.scene.web.WebView;

/**
 * @author Kevin Nguyen (zpp390)
 *
 * 
 */
public class MapController  {
	@FXML
	WebView myWeb; 
	@FXML
	AnchorPane father; 
	@FXML
	Button go_back; 
	public void initialize() {
		// TODO Auto-generated method stub
		StackPane  root1 = new StackPane ();
		root1.setPrefHeight(1000);
		root1.setPrefHeight(1000);
		WebView web = new WebView (); 
		WebEngine engine = web.getEngine();
		Button button = new Button ("Go Back");
		button.setLayoutY(600);
		engine.load("https://www.utsa.edu/visit/main-campus.html");
		root1.getChildren().add(web);
		father.getChildren().add(root1);
		father.getChildren().add(button);
		button.setOnAction(e ->{
			Parent root;
			try {
				root = FXMLLoader.load(getClass().getResource("../view/UserReview.fxml"));
				Main.stage.setScene(new Scene(root, 800, 800));
				Main.stage.show();
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
		});
	}
	public void go_back (ActionEvent Event) {
		try {
			Parent root = FXMLLoader.load(getClass().getResource("../view/UserReview.fxml"));
			Main.stage.setScene(new Scene(root, 800, 800));
			Main.stage.show();

		} catch(Exception e) {
			e.printStackTrace();
		}
	}
}
