package session;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;
import java.sql.SQLException;

public class Connect {

    public static void main(String[] args) {
        // Use try-with-resources to automatically close the connection and statement
        try (Connection con = DriverManager.getConnection("jdbc:oracle:thin:@127.0.0.1:1521:xe", "system", "SYSTEM123");
             Statement st = con.createStatement()) {

            System.out.println("Connection to database established successfully!");

            // 1. Create a table (DDL - use executeUpdate)
            // Using a consistent table name: STUDENT_DEMO
            st.executeUpdate("CREATE TABLE STUDENT_DEMO(name varchar(25), roll int, marks int)");
            System.out.println("Table 'STUDENT_DEMO' created.");

            // 2. Insert data into the table (DML - use executeUpdate)
            st.executeUpdate("INSERT INTO STUDENT_DEMO values('lmn', 2, 21)");
            st.executeUpdate("INSERT INTO STUDENT_DEMO values('abc', 3, 25)");
            st.executeUpdate("INSERT INTO STUDENT_DEMO values('xyz', 4, 22)");
            st.executeUpdate("INSERT INTO STUDENT_DEMO values('def', 5, 27)");
            System.out.println("Data inserted successfully.");

            // 3. Select and display data (DQL - use executeQuery)
            System.out.println("\n--- Initial Data in STUDENT_DEMO ---");
            try (ResultSet rs = st.executeQuery("SELECT * FROM STUDENT_DEMO")) {
                while (rs.next()) {
                    // Access columns by name or index (index starts at 1)
                    System.out.println("Name: " + rs.getString("name") + ", Roll: " + rs.getInt("roll") + ", Marks: " + rs.getInt(3));
                }
            } // ResultSet 'rs' is automatically closed here

            // 4. Update data (DML - use executeUpdate)
            int rowsAffected = st.executeUpdate("UPDATE STUDENT_DEMO SET name='tina' WHERE roll=3");
            System.out.println("\nUpdate executed. Rows affected: " + rowsAffected);

            // 5. Select and display data again to see the update
            System.out.println("\n--- Data after UPDATE ---");
            try (ResultSet rs = st.executeQuery("SELECT * FROM STUDENT_DEMO")) {
                while (rs.next()) {
                    System.out.println("Name: " + rs.getString("name") + ", Roll: " + rs.getInt("roll") + ", Marks: " + rs.getInt("marks"));
                }
            }

            // 6. Alter the table structure (DDL - use executeUpdate)
            st.executeUpdate("ALTER TABLE STUDENT_DEMO ADD admissionDate Date");
            System.out.println("\nTable altered to add 'admissionDate' column.");

            // 7. Drop the table to clean up (DDL - use executeUpdate)
            st.executeUpdate("DROP TABLE STUDENT_DEMO");
            System.out.println("\nTable 'STUDENT_DEMO' dropped successfully.");

        } catch (SQLException ex) {
            System.err.println("Database Error: " + ex.getMessage());
            ex.printStackTrace();
        } catch (Exception ex) {
            System.err.println("An unexpected error occurred: " + ex.getMessage());
            ex.printStackTrace();
        }
    }
}
