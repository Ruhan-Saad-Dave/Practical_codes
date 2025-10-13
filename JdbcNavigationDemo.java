package com.example.jdbc;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class JdbcNavigationDemo {
    private static final String DB_URL = "jdbc:oracle:thin:@172.25.0.26:1521:xe"; 
    private static final String USER = "system";
    private static final String PASS = "Security123"; 

    public static void main(String[] args) {
        try (Connection conn = DriverManager.getConnection(DB_URL, USER, PASS);
             Statement stmt = conn.createStatement()) {

            System.out.println("✅ Connection to the Oracle database established successfully!");
            System.out.println("\n[SETUP] Preparing the database table...");

            try {
            	stmt.executeUpdate("DROP TABLE students CASCADE CONSTRAINTS");
                System.out.println("Dropped existing 'students' table.");
            } catch (SQLException e) {
                if (e.getErrorCode() != 942) {
                    throw e;
                }
            }

            String createTableSQL = "CREATE TABLE students (" +
                                    "roll_no NUMBER PRIMARY KEY, " +
                                    "name VARCHAR2(50), " +
                                    "marks NUMBER)";
            stmt.executeUpdate(createTableSQL);
            System.out.println("'students' table created.");

            // CREATE Operation (Insert)
            System.out.println("\n[CREATE] Inserting records into the table...");
            String insertSQL = "INSERT INTO students (roll_no, name, marks) VALUES (?, ?, ?)";
            try (PreparedStatement pstmt = conn.prepareStatement(insertSQL)) {
                pstmt.setInt(1, 101);
                pstmt.setString(2, "Alice");
                pstmt.setInt(3, 85);
                pstmt.executeUpdate();

                pstmt.setInt(1, 102);
                pstmt.setString(2, "Bob");
                pstmt.setInt(3, 92);
                pstmt.executeUpdate();
                
                pstmt.setInt(1, 103);
                pstmt.setString(2, "Charlie");
                pstmt.setInt(3, 78);
                pstmt.executeUpdate();
            }
            System.out.println("3 records inserted.");

            // READ Operation (Select / Navigate)
            System.out.println("\n[READ] Displaying all student data:");
            displayAllStudents(conn);

            // UPDATE Operation
            System.out.println("\n[UPDATE] Updating Bob's marks to 95...");
            String updateSQL = "UPDATE students SET marks = ? WHERE name = ?";
            try (PreparedStatement pstmt = conn.prepareStatement(updateSQL)) {
                pstmt.setInt(1, 95);
                pstmt.setString(2, "Bob");
                int rowsAffected = pstmt.executeUpdate();
                System.out.println(rowsAffected + " row(s) updated.");
            }
            
            System.out.println("\n[READ] Displaying data after update:");
            displayAllStudents(conn);
            
            // DELETE Operation 
            System.out.println("\n[DELETE] Deleting Charlie from the records...");
            String deleteSQL = "DELETE FROM students WHERE roll_no = ?";
             try (PreparedStatement pstmt = conn.prepareStatement(deleteSQL)) {
                pstmt.setInt(1, 103);
                int rowsAffected = pstmt.executeUpdate();
                System.out.println(rowsAffected + " row(s) deleted.");
            }
            
            System.out.println("\n[READ] Final data in the table:");
            displayAllStudents(conn);

        } catch (SQLException e) {
            System.err.println("❌ Database Error: " + e.getMessage());
            System.err.println("SQL State: " + e.getSQLState());
            System.err.println("Error Code: " + e.getErrorCode());
            e.printStackTrace();
        }
        
        System.out.println("\n✅ Demo finished. Connection is automatically closed.");
    }

    private static void displayAllStudents(Connection connection) throws SQLException {
        String selectAllSQL = "SELECT roll_no, name, marks FROM students";
        try (Statement stmt = connection.createStatement();
             ResultSet rs = stmt.executeQuery(selectAllSQL)) {
            
            System.out.println("-----------------------------");
            System.out.printf("%-10s %-15s %-5s\n", "ROLL NO", "NAME", "MARKS");
            System.out.println("-----------------------------");

            while (rs.next()) {
                int roll = rs.getInt("roll_no");
                String name = rs.getString("name");
                int marks = rs.getInt("marks");
                System.out.printf("%-10d %-15s %-5d\n", roll, name, marks);
            }
            System.out.println("-----------------------------");
        }
    }
}
