package com.example;

import com.mongodb.client.MongoClients;
import com.mongodb.client.MongoClient;
import com.mongodb.client.MongoDatabase;
import com.mongodb.client.MongoCollection;
import com.mongodb.client.MongoCursor;
import com.mongodb.client.model.Filters;
import com.mongodb.client.result.DeleteResult;
import com.mongodb.client.result.InsertOneResult;
import com.mongodb.client.result.UpdateResult;

import org.bson.Document;
import java.util.Scanner;

public class MongoDbCrudExample {

    public static void main(String[] args) {
        // --- 1. Connection Details ---
        String connectionString = "mongodb://localhost:27017";

        // --- 2. Connect to MongoDB using try-with-resources ---
        try (MongoClient mongoClient = MongoClients.create(connectionString);
             Scanner scanner = new Scanner(System.in)) {

            System.out.println("✅ Connection to MongoDB successful!");
            MongoDatabase database = mongoClient.getDatabase("university");
            MongoCollection<Document> collection = database.getCollection("students");
            
            // We removed collection.drop() so data persists between runs.

            boolean exit = false;
            while (!exit) {
                printMenu();
                System.out.print("👉 Enter your choice: ");
                int choice = scanner.nextInt();
                scanner.nextLine(); // Consume the newline character left by nextInt()

                switch (choice) {
                    case 1:
                        insertStudent(collection, scanner);
                        break;
                    case 2:
                        readAllStudents(collection);
                        break;
                    case 3:
                        updateStudent(collection, scanner);
                        break;
                    case 4:
                        deleteStudent(collection, scanner);
                        break;
                    case 5:
                        exit = true;
                        System.out.println("👋 Exiting the application. Goodbye!");
                        break;
                    default:
                        System.out.println("❌ Invalid choice. Please try again.");
                }
            }

        } catch (Exception e) {
            System.err.println("An error occurred: " + e);
        }
    }

    private static void printMenu() {
        System.out.println("\n--- Student Database Menu ---");
        System.out.println("1. ➕ Add a new student (Create)");
        System.out.println("2. 📖 View all students (Read)");
        System.out.println("3. ✏️ Update a student's info (Update)");
        System.out.println("4. 🗑️ Delete a student (Delete)");
        System.out.println("5. 🚪 Exit");
    }

    /**
     * CREATE operation: Inserts a new student document into the collection.
     */
    private static void insertStudent(MongoCollection<Document> collection, Scanner scanner) {
        System.out.println("\n--- Add a New Student ---");
        System.out.print("Enter student name: ");
        String name = scanner.nextLine();
        System.out.print("Enter student major: ");
        String major = scanner.nextLine();
        System.out.print("Enter student GPA: ");
        double gpa = scanner.nextDouble();
        scanner.nextLine(); // Consume newline

        Document student = new Document("name", name)
                .append("major", major)
                .append("gpa", gpa);

        InsertOneResult result = collection.insertOne(student);
        System.out.println("✅ Student inserted successfully with ID: " + result.getInsertedId());
    }

    /**
     * READ operation: Finds and displays all documents in the collection.
     */
    private static void readAllStudents(MongoCollection<Document> collection) {
        System.out.println("\n--- List of All Students ---");
        if (collection.countDocuments() == 0) {
            System.out.println("No students found in the database.");
            return;
        }
        
        MongoCursor<Document> cursor = collection.find().iterator();
        while (cursor.hasNext()) {
            System.out.println(cursor.next().toJson());
        }
    }

    /**
     * UPDATE operation: Finds a student by name and updates their information.
     */
    private static void updateStudent(MongoCollection<Document> collection, Scanner scanner) {
        System.out.println("\n--- Update a Student's Info ---");
        System.out.print("Enter the name of the student to update: ");
        String nameToUpdate = scanner.nextLine();
        
        System.out.print("Enter the new major: ");
        String newMajor = scanner.nextLine();
        System.out.print("Enter the new GPA: ");
        double newGpa = scanner.nextDouble();
        scanner.nextLine(); // Consume newline

        Document query = new Document("name", nameToUpdate);
        Document update = new Document("$set", new Document("major", newMajor).append("gpa", newGpa));

        UpdateResult result = collection.updateOne(query, update);
        if (result.getModifiedCount() > 0) {
            System.out.println("✅ Student information updated successfully.");
        } else {
            System.out.println("⚠️ No student found with that name.");
        }
    }

    /**
     * DELETE operation: Finds a student by name and deletes them.
     */
    private static void deleteStudent(MongoCollection<Document> collection, Scanner scanner) {
        System.out.println("\n--- Delete a Student ---");
        System.out.print("Enter the name of the student to delete: ");
        String nameToDelete = scanner.nextLine();

        DeleteResult result = collection.deleteOne(Filters.eq("name", nameToDelete));
        if (result.getDeletedCount() > 0) {
            System.out.println("✅ Student deleted successfully.");
        } else {
            System.out.println("⚠️ No student found with that name.");
        }
    }
}
