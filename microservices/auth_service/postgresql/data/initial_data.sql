INSERT INTO
  auth_schema.users (email, password)
VALUES
  ('user-from-initial_data.sql', 42) ON CONFLICT (email) DO NOTHING;
