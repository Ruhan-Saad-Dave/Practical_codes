Create table employees_21(job_id varchar(10), name varchar(10), manager_id number(3)); 
insert into employees_21 values('CLERK','ABC',111); 
insert into employees_21 values('PEON','XYZ',110); 
insert into employees_21 values('CLERK','PQR',121); 
insert into employees_21 values('CLERK','ZXC',121); 
select * from employees_21; 
SET SERVEROUT ON; 
DECLARE 
CURSOR c1 IS 
SELECT name, job_id FROM employees_21 
WHERE job_id LIKE '%CLERK%' AND manager_id > 120 
ORDER BY name; 
BEGIN 
FOR item IN c1 
LOOP 
DBMS_OUTPUT.PUT_LINE('Name = ' || item.name || ', Job = ' || item.job_id); 
END LOOP; 
END;