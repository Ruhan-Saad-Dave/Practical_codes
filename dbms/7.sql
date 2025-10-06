-- Clean up existing objects to allow the script to be re-run
-- The "exception when others then null" part handles cases where the tables don't exist yet
BEGIN
   EXECUTE IMMEDIATE 'DROP TABLE library_audit';
EXCEPTION
   WHEN OTHERS THEN
      IF SQLCODE != -942 THEN
         RAISE;
      END IF;
END;
/

BEGIN
   EXECUTE IMMEDIATE 'DROP TABLE borrower';
EXCEPTION
   WHEN OTHERS THEN
      IF SQLCODE != -942 THEN
         RAISE;
      END IF;
END;
/

-- Create the tables
-- Note: Matched the data types in library_audit to borrower to prevent errors
CREATE TABLE borrower (
    rollin      INT,
    name        VARCHAR(30),
    dateofissue DATE,
    nameofbook  VARCHAR(30),
    status      CHAR(10)
);

CREATE TABLE library_audit (
    rollin      INT,
    name        VARCHAR(30),
    dateofissue DATE,
    nameofbook  VARCHAR(30),
    status      CHAR(10),
    ts          TIMESTAMP
);

-- Insert initial data one row at a time, using TO_DATE for explicit date conversion
INSERT INTO borrower VALUES (101, 'abc', TO_DATE('2017-07-16', 'YYYY-MM-DD'), 'dbms', 'r');
INSERT INTO borrower VALUES (102, 'abc1', TO_DATE('2017-07-16', 'YYYY-MM-DD'), 'cn', 'i');
INSERT INTO borrower VALUES (103, 'abc3', TO_DATE('2017-07-18', 'YYYY-MM-DD'), 'toc', 'i');
INSERT INTO borrower VALUES (104, 'abc4', TO_DATE('2017-07-20', 'YYYY-MM-DD'), 'ds', 'i');
INSERT INTO borrower VALUES (105, 'abc5', TO_DATE('2017-07-23', 'YYYY-MM-DD'), 'daa', 'r');
INSERT INTO borrower VALUES (106, 'nisha', TO_DATE('2017-08-10', 'YYYY-MM-DD'), 'splm', 'r');

COMMIT;

-- Trigger for logging new inserts
-- Using "CREATE OR REPLACE" is good practice to avoid errors if the trigger already exists
CREATE OR REPLACE TRIGGER after_insert
AFTER INSERT ON borrower
FOR EACH ROW
BEGIN
    INSERT INTO library_audit VALUES(
        :new.rollin,
        :new.name,
        :new.dateofissue,
        :new.nameofbook,
        :new.status,
        current_timestamp
    );
END;
/

-- Trigger for logging deletions
CREATE OR REPLACE TRIGGER after_delete
AFTER DELETE ON borrower
FOR EACH ROW
BEGIN
    INSERT INTO library_audit VALUES(
        :old.rollin,
        :old.name,
        :old.dateofissue,
        :old.nameofbook,
        :old.status,
        current_timestamp
    );
END;
/

-- Trigger for logging updates (logs the state of the row *before* the update)
CREATE OR REPLACE TRIGGER after_update
AFTER UPDATE ON borrower
FOR EACH ROW
BEGIN
    INSERT INTO library_audit VALUES(
        :old.rollin,
        :old.name,
        :old.dateofissue,
        :old.nameofbook,
        :old.status,
        current_timestamp
    );
END;
/


-- =================================================================
-- Testing the Triggers
-- =================================================================

-- 1. Test the AFTER INSERT trigger
PROMPT -- Initial data in borrower and library_audit
SELECT * FROM borrower;
SELECT * FROM library_audit;

PROMPT -- Inserting a new record...
INSERT INTO borrower VALUES(107, 'ada', TO_DATE('2017-08-10', 'YYYY-MM-DD'), 'dbms', 'i');

PROMPT -- Data after insert (note the new row in both tables)
SELECT * FROM borrower;
SELECT * FROM library_audit;


-- 2. Test the AFTER DELETE trigger
PROMPT -- Deleting record where rollin = 105...
DELETE FROM borrower WHERE rollin = 105;

PROMPT -- Data after delete (note the deleted row is now in the audit table)
SELECT * FROM borrower;
SELECT * FROM library_audit;


-- 3. Test the AFTER UPDATE trigger
PROMPT -- Updating record where rollin = 104...
UPDATE borrower SET status = 'r' WHERE rollin = 104;

PROMPT -- Data after update (note the *old* state of row 104 is now in the audit table)
SELECT * FROM borrower;
SELECT * FROM library_audit;

COMMIT;