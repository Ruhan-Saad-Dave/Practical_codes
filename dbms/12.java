import com.mongodb.client.MongoClients;
import com.mongodb.client.MongoClient;
import com.mongodb.client.MongoDatabase;
import com.mongodb.client.MongoCollection;
import com.mongodb.client.MongoCursor;
import com.mongodb.client.result.DeleteResult;
import com.mongodb.client.result.UpdateResult;

import org.bson.Document;
import static com.mongodb.client.model.Filters.eq;
import static com.mongodb.client.model.Updates.set;

import com.mongodb.MongoException;

public class MongoJavaCRUD {

    public static void main(String[] args) {
        // --- 1. Connection String ---
        // Replace with your MongoDB deployment's connection string.
        // For a local server, this is typically the default.
        String connectionString = "mongodb://localhost:27017";

        // --- 2. Connect to MongoDB using a try-with-resources block ---
        // This ensures that the connection is automatically closed.
        try (MongoClient mongoClient = MongoClients.create(connectionString)) {

            // --- 3. Get the Database ---
            // If the database does not exist, MongoDB will create it when you first store data.
            MongoDatabase database = mongoClient.getDatabase("university");
            System.out.println("✅ Successfully connected to database: " + database.getName());

            // --- 4. Get the Collection ---
            // If the collection does not exist, MongoDB will create it.
            MongoCollection<Document> collection = database.getCollection("students");
            System.out.println("✅ Successfully selected collection: " + collection.getNamespace());
            
            // Clean up the collection before starting to ensure a fresh run
            collection.deleteMany(new Document());
            System.out.println("\n🧹 Cleaned up 'students' collection for a fresh start.");

            // --- 5. ADD (Create/Insert) a Document ---
            System.out.println("\n--- ➕ ADD OPERATION ---");
            Document student = new Document("name", "John Doe")
                    .append("student_id", "12345")
                    .append("major", "Computer Science")
                    .append("gpa", 3.8);

            collection.insertOne(student);
            System.out.println("✅ Document inserted successfully: " + student.toJson());

            // --- 6. READ (Find) Documents ---
            System.out.println("\n--- 📖 READ OPERATIONS ---");
            // Find a specific document
            System.out.println("🔍 Finding the student named 'John Doe':");
            Document foundStudent = collection.find(eq("name", "John Doe")).first();
            if (foundStudent != null) {
                System.out.println("✅ Found student: " + foundStudent.toJson());
            } else {
                System.out.println("❌ Student not found.");
            }

            // Find all documents in the collection
            System.out.println("\n🔍 Listing all students in the collection:");
            try (MongoCursor<Document> cursor = collection.find().iterator()) {
                while (cursor.hasNext()) {
                    System.out.println(" -> " + cursor.next().toJson());
                }
            }

            // --- 7. EDIT (Update) a Document ---
            System.out.println("\n--- ✏️ EDIT/UPDATE OPERATION ---");
            System.out.println("Updating John Doe's GPA to 3.9...");
            UpdateResult updateResult = collection.updateOne(
                eq("name", "John Doe"), // Filter: find the document to update
                set("gpa", 3.9)       // Update operation: set the new value
            );

            System.out.println("✅ Documents matched: " + updateResult.getMatchedCount());
            System.out.println("✅ Documents modified: " + updateResult.getModifiedCount());

            // Verify the update
            System.out.println("🔍 Verifying the update for John Doe:");
            Document updatedStudent = collection.find(eq("name", "John Doe")).first();
            System.out.println("✅ Updated student details: " + updatedStudent.toJson());

            // --- 8. DELETE a Document ---
            System.out.println("\n--- ❌ DELETE OPERATION ---");
            System.out.println("Deleting the student named 'John Doe'...");
            DeleteResult deleteResult = collection.deleteOne(eq("name", "John Doe"));
            System.out.println("✅ Documents deleted: " + deleteResult.getDeletedCount());
            
            // Verify the deletion by trying to find the student again
            System.out.println("🔍 Verifying the deletion...");
            if (collection.find(eq("name", "John Doe")).first() == null) {
                 System.out.println("✅ Student 'John Doe' has been successfully deleted.");
            } else {
                 System.out.println("❌ Deletion failed.");
            }

        } catch (MongoException e) {
            System.err.println("An error occurred while connecting to MongoDB: " + e.getMessage());
            e.printStackTrace();
        }
    }
}
