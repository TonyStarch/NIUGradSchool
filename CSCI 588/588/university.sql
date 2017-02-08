--
-- DESCRIPTION
--   This script creates the SQL*Plus demonstration tables in the
--   current schema.  It should be STARTed by each user wishing to
--   access the tables.  To remove the tables use the demodrop.sql
--   script.
--
--  USAGE
--       SQL> START demobld.sql
--
--

-- turns off messages (e.g., table created, row created, etc.)
SET TERMOUT OFF

-- order of these DROPS matters because of
-- foreign keys that are specified.
DROP TABLE Enrolled;
DROP TABLE Student;
DROP TABLE Taught;
DROP TABLE Faculty;

SET TERMOUT ON
PROMPT Building STUDENT ...
SET TERMOUT OFF

CREATE TABLE Student 
    (
	ID 	NUMBER, 
	Name	VARCHAR2(10)	NOT NULL,
	PRIMARY KEY (ID)
    );

INSERT INTO Student VALUES (1, 'Adam');
INSERT INTO Student VALUES (2, 'Brett');
INSERT INTO Student VALUES (3, 'Chris');
INSERT INTO Student VALUES (4, 'Dudley');
INSERT INTO Student VALUES (5, 'Ellie');
INSERT INTO Student VALUES (6, 'Frank');

SET TERMOUT ON
PROMPT Building FACULTY ...
SET TERMOUT OFF

CREATE TABLE Faculty
    (
	ID		NUMBER,
	Name		VARCHAR2(10)	NOT NULL,
	Dept		VARCHAR2(10)	NOT NULL,
	SpouseID	NUMBER,
	PRIMARY KEY (ID)
    );

INSERT INTO Faculty VALUES(11, 'Dr. Adams',  'CSCI', 12);
INSERT INTO Faculty VALUES(12, 'Dr. Bryce',  'CSCI', 11);
INSERT INTO Faculty VALUES(13, 'Dr. Cook',   'CSCI', 24);
INSERT INTO Faculty VALUES(24, 'Dr. Drake',  'MATH', 13);
INSERT INTO Faculty VALUES(25, 'Dr. Engles', 'MATH', NULL);
INSERT INTO Faculty VALUES(26, 'Dr. Fink',   'MATH', 3);

SET TERMOUT ON
PROMPT Building ENROLLED ...
SET TERMOUT OFF

CREATE TABLE Enrolled
    (
	Dept		VARCHAR2(10),
	CourseNum	NUMBER,
	Sem		VARCHAR2(10),
	StudentID	NUMBER,
	GradeEarned	NUMBER		NOT NULL,
	PRIMARY KEY (Dept,CourseNum,Sem,StudentID)
    );

-- Fall 2000
INSERT INTO Enrolled VALUES ('CSCI', 360, 'Fall 2000',  1, 4);
INSERT INTO Enrolled VALUES ('CSCI', 360, 'Fall 2000',  2, 3);
INSERT INTO Enrolled VALUES ('CSCI', 360, 'Fall 2000',  3, 0);
INSERT INTO Enrolled VALUES ('CSCI', 360, 'Fall 2000', 12, 4);
INSERT INTO Enrolled VALUES ('MATH', 229, 'Fall 2000',  3, 4);
INSERT INTO Enrolled VALUES ('MATH', 229, 'Fall 2000',  1, 3);
-- Fall 2001
INSERT INTO Enrolled VALUES ('CSCI', 360, 'Fall 2001',  3, 2);
INSERT INTO Enrolled VALUES ('CSCI', 360, 'Fall 2001',  5, 3);
INSERT INTO Enrolled VALUES ('MATH', 229, 'Fall 2001', 13, 4);
INSERT INTO Enrolled VALUES ('MATH', 229, 'Fall 2001',  2, 3);
-- Fall 2002
INSERT INTO Enrolled VALUES ('CSCI', 588, 'Fall 2002',  3, 2);
INSERT INTO Enrolled VALUES ('CSCI', 588, 'Fall 2002', 24, 4);
-- Fall 2003
INSERT INTO Enrolled VALUES ('MATH', 210, 'Fall 2003',  4, 4);
INSERT INTO Enrolled VALUES ('MATH', 210, 'Fall 2003',  5, 3);
INSERT INTO Enrolled VALUES ('MATH', 240, 'Fall 2003',  1, 2);
INSERT INTO Enrolled VALUES ('MATH', 240, 'Fall 2003',  5, 3);

SET TERMOUT ON
PROMPT Building TAUGHT ...
SET TERMOUT OFF

CREATE TABLE Taught
    (
	Dept		VARCHAR2(10),
	CourseNum	NUMBER	,
	Sem		VARCHAR2(10),
	FacultyID	NUMBER,
	PRIMARY KEY (Dept,CourseNum,Sem,FacultyID),
	FOREIGN KEY (FacultyID) REFERENCES Faculty(ID)
    );

-- Fall 2000
INSERT INTO Taught VALUES ('CSCI', 360, 'Fall 2000', 11);
INSERT INTO Taught VALUES ('MATH', 229, 'Fall 2000', 26);
-- Fall 2001
INSERT INTO Taught VALUES ('CSCI', 360, 'Fall 2001', 12);
INSERT INTO Taught VALUES ('MATH', 229, 'Fall 2001', 24);
-- Fall 2002
INSERT INTO Taught VALUES ('CSCI', 588, 'Fall 2002', 13);
-- Fall 2003
INSERT INTO Taught VALUES ('MATH', 210, 'Fall 2003', 25);
INSERT INTO Taught VALUES ('MATH', 240, 'Fall 2003', 25);

EXIT
