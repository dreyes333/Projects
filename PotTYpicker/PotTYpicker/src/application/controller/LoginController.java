package application.controller;

import java.io.IOException;
import java.net.URL;
import java.util.ResourceBundle;

import application.Main;
import application.model.User;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.control.PasswordField;
import javafx.scene.control.TextField;

/**
 * @author Kevin Nguyen (zpp390)
 *
 * 
 */
public class LoginController implements Initializable { 
	@FXML
	TextField username; 
	@FXML
	PasswordField password;
	@FXML
	Label message;
	
	public static User user;
	
	public void initialize(URL location, ResourceBundle resources) {
		LoginController.user = null;
	}
	
	/**
	 * Logs the user in after validating credentials and shows them the next view
	 * @param event The Login button is pressed
	 */
	public void login (ActionEvent event) {
		LoginController.user = new User(username.getText(), password.getText());
		try {
			if (!LoginController.user.validate()) {
				message.setText("Your username/password is incorrect.");
			} else {
				gotoSelection();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * Creates a new user by called User.newUser() with arguments
	 * from username and password fields
	 * @param event The register button is pressed
	 */
	public void register(ActionEvent event) {
		try {
			LoginController.user = User.newUser(username.getText(), password.getText());
			message.setText("Successfully registered");
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	private void gotoSelection() {
		try {
			Parent root = FXMLLoader.load(getClass().getResource("../view/Selection.fxml"));
			Main.stage.setScene(new Scene(root, 800, 800));
			Main.stage.show();

		} catch(Exception e) {
			e.printStackTrace();
		}
	}
}
