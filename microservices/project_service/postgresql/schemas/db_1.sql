DROP SCHEMA IF EXISTS project_schema CASCADE;

CREATE SCHEMA IF NOT EXISTS project_schema;

CREATE TABLE IF NOT EXISTS project_schema.projects (
  id SERIAL PRIMARY KEY,
  user_id SERIAL NOT NULL,
  name TEXT NOT NULL
);
