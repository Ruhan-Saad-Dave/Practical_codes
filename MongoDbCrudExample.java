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

public class MongoDbCrudExample {

    public static void main(String[] args) {
        // --- 1. Connection Details ---
        String connectionString = "mongodb://localhost:27017";

        // --- 2. Connect to MongoDB ---
        try (MongoClient mongoClient = MongoClients.create(connectionString)) {
            System.out.println("Connection to MongoDB successful!");
            MongoDatabase database = mongoClient.getDatabase("university");
            MongoCollection<Document> collection = database.getCollection("students");
            collection.drop();
            System.out.println("\nDropped existing 'students' collection.");

            // --- 3. ADD (Create) Operation ---
            System.out.println("\n--- ADD (Create) Operation ---");
            Document student = new Document("name", "John Doe")
                    .append("major", "Computer Science")
                    .append("gpa", 3.8);
            InsertOneResult insertResult = collection.insertOne(student);
            System.out.println("📄 Inserted a single document with ID: " + insertResult.getInsertedId().asObjectId().getValue());

            // --- 4. READ (Find) Operation ---
            System.out.println("\n--- READ (Find) Operation ---");
            System.out.println("Finding all documents in the collection:");
            MongoCursor<Document> cursor = collection.find().iterator();
            while (cursor.hasNext()) {
                System.out.println(cursor.next().toJson());
            }

            // --- 5. EDIT (Update) Operation ---
            System.out.println("\n--- EDIT (Update) Operation ---");
            Document query = new Document("name", "John Doe");
            Document update = new Document("$set", new Document("gpa", 3.9).append("status", "Active"));
            UpdateResult updateResult = collection.updateOne(query, update);
            System.out.println("Matched " + updateResult.getMatchedCount() + " document(s) and modified " + updateResult.getModifiedCount() + " document(s).");
            System.out.println("Finding the updated document:");
            Document updatedStudent = collection.find(Filters.eq("name", "John Doe")).first();
            if(updatedStudent != null) {
                System.out.println(updatedStudent.toJson());
            }

            // --- 6. DELETE Operation ---
            /*System.out.println("\n--- DELETE Operation ---");
            DeleteResult deleteResult = collection.deleteOne(Filters.eq("name", "John Doe"));
            System.out.println("Deleted " + deleteResult.getDeletedCount() + " document(s).");
            long count = collection.countDocuments();
            System.out.println("Number of documents in collection after deletion: " + count);
			*/
        } catch (Exception e) {
            System.err.println("An error occurred: " + e);
        }
    }
}
