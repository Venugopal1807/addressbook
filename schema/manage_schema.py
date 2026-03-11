"""
Schema management CLI tool for the Addressbook database.

Wraps Alembic commands for convenient schema migration management.
Use this script to initialize, create, and apply database migrations.

Usage:
    python manage_schema.py init       Initialize a new database
    python manage_schema.py migrate    Auto-generate a new migration
    python manage_schema.py upgrade    Apply all pending migrations
    python manage_schema.py downgrade  Rollback the last migration
    python manage_schema.py current    Show current migration revision
    python manage_schema.py history    Show migration history
"""

import os
import sys
import argparse

from alembic.config import Config
from alembic import command


def get_alembic_config() -> Config:
    """
    Returns a configured Alembic Config object.

    Points to the alembic.ini in the migrations/ directory and ensures
    the database URL points to the correct SQLite file.
    """
    migrations_dir = os.path.join(
        os.path.dirname(os.path.abspath(__file__)), "..", "migrations"
    )
    config_path = os.path.join(migrations_dir, "alembic.ini")

    if not os.path.exists(config_path):
        print(f"Error: alembic.ini not found at {config_path}")
        sys.exit(1)

    config = Config(config_path)
    config.set_main_option("script_location", migrations_dir)
    return config


def cmd_init(config: Config) -> None:
    """Initialize the database with the current schema."""
    command.upgrade(config, "head")
    print("Database initialized successfully.")


def cmd_migrate(config: Config, message: str = "auto migration") -> None:
    """Auto-generate a new migration based on model changes."""
    command.revision(config, autogenerate=True, message=message)
    print(f"Migration created: {message}")


def cmd_upgrade(config: Config) -> None:
    """Apply all pending migrations."""
    command.upgrade(config, "head")
    print("Database upgraded to latest revision.")


def cmd_downgrade(config: Config) -> None:
    """Rollback the last migration."""
    command.downgrade(config, "-1")
    print("Database downgraded by one revision.")


def cmd_current(config: Config) -> None:
    """Show the current migration revision."""
    command.current(config, verbose=True)


def cmd_history(config: Config) -> None:
    """Show migration revision history."""
    command.history(config, verbose=True)


def main() -> None:
    """Parse arguments and dispatch to the appropriate command."""
    parser = argparse.ArgumentParser(
        description="Addressbook database schema management tool."
    )
    parser.add_argument(
        "command",
        choices=["init", "migrate", "upgrade", "downgrade", "current", "history"],
        help="Schema management command to execute.",
    )
    parser.add_argument(
        "-m",
        "--message",
        default="auto migration",
        help="Migration message (used with 'migrate' command).",
    )

    args = parser.parse_args()
    config = get_alembic_config()

    commands = {
        "init": lambda: cmd_init(config),
        "migrate": lambda: cmd_migrate(config, args.message),
        "upgrade": lambda: cmd_upgrade(config),
        "downgrade": lambda: cmd_downgrade(config),
        "current": lambda: cmd_current(config),
        "history": lambda: cmd_history(config),
    }

    commands[args.command]()


if __name__ == "__main__":
    main()
