# Laboratory Work #1

## Short description
 **Topic:** database design and familiarization with basic operations of the PostgreSQL DBMS.
 **Purpose of work:** acquiring database design skills and practical skills in creating relational databases using PostgreSQL.

## Key Tasks
   1. Develop an "entity-relationship" model of the subject area chosen by the student independently, in accordance with the "ER-model requirements" item.
   2. Convert the developed model into a PostgreSQL database (table) schema.
   3. Perform database schema normalization to the third normal form (3NF).
   4. Familiarize yourself with the PostgreSQL and pgAdmin 4 tools and enter several rows of data into each table using pgAdmin 4.

## Project Files
-   **BDLAB1_1.drawio.png** - The Entity-Relationship diagram (Crow's Foot notation).
-   **BDLAB1_2.drawio.png** - The logical database schema diagram.
-   **lab1.pdf** - The complete project report.
-   **lab1.sql** - An additional SQL script with various queries.
-   **lab1.zip** - An archive containing the primary SQL scripts (`schema.sql`, `data.sql`) and the pgAdmin project file (`lab1.pgerd`).
-   **lab1_bd_zu_2025_2026.docx** - The assignment document with the task description and requirements.
-   **lab1_database_diagam.png** - The database schema diagram generated from pgAdmin 4.

## How to Deploy
To recreate this database on your local machine, follow these steps:
1.  **Create a new database** in your PostgreSQL instance.
2.  **Run the `schema.sql` script.** This will create the required tables and relationships.
3.  **Run the `data.sql` script.** This will populate the tables with the initial data.
