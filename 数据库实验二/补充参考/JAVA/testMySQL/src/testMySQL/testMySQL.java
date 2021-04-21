package testMySQL;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class testMySQL {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		String url = "jdbc:mysql://127.0.0.1:3306/company?serverTimezone=GMT%2B8&useSSL=false";
		String user = "root";
		String pass = "MySQL1234";

		try {
			Class.forName("com.mysql.cj.jdbc.Driver");
			Connection con = DriverManager.getConnection(url, user, pass);

			Statement state = con.createStatement();
			String querySql = "select * from user";
			ResultSet result = state.executeQuery(querySql);

			while (result.next()) {
				System.out.println("username£º" + result.getString("user"));
				System.out.println("password£º" + result.getString("password"));
				System.out.println();
			}
			result.close();
			state.close();
			con.close();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}
}