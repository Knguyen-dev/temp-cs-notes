# Database Migrations 

### What are "database migrations"?
A way to manage changes to your database schema over time. As the app evolves you may need to add new things, add new tables, modify existing ones, or delete data. Migrations just makes sure these changes are applied consistently.
- Good for tracking changes
- Recovery: Provide a good way to revert changes and recover data.

### Typical structure of a database migration file
Database migrations are stored in files containing the changes to the database that the file made. The files have a filename convention where you prefix it with a timestamp:

`20230915120000_create_users_table`

It's basically a numerical prefix, as files prefixed with smaller numbers go first. There's no strict rule that we have to use timestamps, it's just that timestamps are a convenient way to maintain order. Each database migration file should have two methods:
  - Up: How to apply changes to the database
  - Down: How to reverse the changes made by this file.

### Manual vs Automatic Migrations 
- **Handling Migrations Manually:** You're writing SQL files to alter tables, drop columns, delete data, etc. Then you're making sure the database is open and ready for changes. In order to revert your changes, you're going to write another SQL script to revert things into how they were.

```sql
-- Up: Apply migration
CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    username VARCHAR(100) UNIQUE NOT NULL,
    email VARCHAR(100),
    created_at TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP
);

-- Down: Reverse migration; have this section ready in case you want to reverse the changes in the future.
-- If you want to revert changes. Just comment out the top section and uncomment this section. 
-- So in your files, either up or down is disabled, you can't have both sections running because then your database isn't changing.
-- DROP TABLE users;
```

Then apply the changes, here we applied it via the command line:
```bash 
psql -U myUser -d myDatabase -f migrations/1_create_users_table.sql
```

Or you can create a python program that accepts the migration file path and uses `asyncpg` to apply it:
```Python
import asyncpg
import asyncio

async def apply_migration(file_path):
    conn = await asyncpg.connect("postgresql://user:password@localhost/dbname")
    with open(file_path, 'r') as file:
        migration_sql = file.read()
    await conn.execute(migration_sql)
    await conn.close()
asyncio.run(apply_migration("migrations/20230915120000_create_users_table.sql"))

```

- **Automated Migration Tools:** For larger projects you may use an ORM, and luckily we're getting a little more programmatic, and so we can use automated migration tools. These make it easier to track which migrations have been applied, they automate generating the scripts for changes, and they let you revert cleanly.