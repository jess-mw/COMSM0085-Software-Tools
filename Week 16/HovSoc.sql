DROP TABLE IF EXISTS Member;
DROP TABLE IF EXISTS Committee;
DROP TABLE IF EXISTS Event;

CREATE TABLE HovMember (
   id INTEGER PRIMARY KEY AUTO_INCREMENT,
   email VARCHAR(100) UNIQUE NOT NULL,
   member_name VARCHAR(100) NOT NULL,
   student_number INTEGER NULL,
   skill_level INTEGER NULL
);

CREATE TABLE Committee (
   id INTEGER PRIMARY KEY AUTO_INCREMENT,
   role VARCHAR(100) UNIQUE NOT NULL,
   member INTEGER NOT NULL,
   FOREIGN KEY (member) REFERENCES HovMember(id)
);

CREATE TABLE Event (
   event_date DATE NOT NULL,
   location VARCHAR(100) NOT NULL,
   event_description VARCHAR(300) NULL,
   event_name VARCHAR(100) NOT NULL,
   organiser INTEGER NOT NULL,
   PRIMARY KEY (event_date, location),
   FOREIGN KEY (organiser) REFERENCES HovMember(id)
);
