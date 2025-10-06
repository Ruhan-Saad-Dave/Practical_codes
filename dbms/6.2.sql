SET SERVEROUTPUT ON;

BEGIN
   EXECUTE IMMEDIATE 'DROP TABLE new_class';
   EXECUTE IMMEDIATE 'DROP TABLE old_class';
EXCEPTION
   WHEN OTHERS THEN
      IF SQLCODE != -942 THEN
         RAISE;
      END IF;
END;
/

CREATE TABLE old_class(
    roll NUMBER(10) PRIMARY KEY,
    name VARCHAR2(20)
);

CREATE TABLE new_class(
    roll NUMBER(10) PRIMARY KEY,
    name VARCHAR2(20)
);

INSERT INTO old_class VALUES (101, 'Amit');
INSERT INTO old_class VALUES (102, 'Priya');
INSERT INTO old_class VALUES (103, 'Rahul');
INSERT INTO old_class VALUES (104, 'Sneha');

INSERT INTO new_class VALUES (102, 'Priya');
INSERT INTO new_class VALUES (105, 'Kiran');

COMMIT;

SELECT * FROM old_class;

SELECT * FROM new_class;

DECLARE

    CURSOR crsr_old_class IS
        SELECT roll, name FROM old_class;

    CURSOR crsr_check_new (p_roll NUMBER) IS
        SELECT roll FROM new_class WHERE roll = p_roll;

    v_old_roll old_class.roll%TYPE;
    v_old_name old_class.name%TYPE;

    v_check_roll new_class.roll%TYPE;

BEGIN
    OPEN crsr_old_class;

    LOOP
        FETCH crsr_old_class INTO v_old_roll, v_old_name;
        EXIT WHEN crsr_old_class%NOTFOUND;

        OPEN crsr_check_new(v_old_roll);

        FETCH crsr_check_new INTO v_check_roll;

        IF crsr_check_new%FOUND THEN
            DBMS_OUTPUT.PUT_LINE('SKIPPING: Student with Roll No ' || v_old_roll || ' (' || v_old_name || ') already exists.');
        ELSE
            DBMS_OUTPUT.PUT_LINE('INSERTING: Student with Roll No ' || v_old_roll || ' (' || v_old_name || ') does not exist. Adding to new_class.');
            INSERT INTO new_class (roll, name) VALUES (v_old_roll, v_old_name);
        END IF;

        CLOSE crsr_check_new;

    END LOOP;

    CLOSE crsr_old_class;
    
    COMMIT;

END;
/

SELECT * FROM new_class ORDER BY roll;
