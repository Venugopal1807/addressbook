#!/usr/bin/env python3
"""
CLI wrapper around Alembic for managing database schema migrations.

Usage:
    python manage_schema.py init       - create db and apply all migrations
    python manage_schema.py migrate    - auto-generate new migration from model changes
    python manage_schema.py upgrade    - apply pending migrations
    python manage_schema.py downgrade  - rollback last migration
    python manage_schema.py current    - show current revision
    python manage_schema.py history    - list all migrations
"""

import os
import sys
import argparse

from alembic.config import Config
from alembic import command

# paths relative to this script
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_DIR = os.path.dirname(BASE_DIR)
ALEMBIC_INI = os.path.join(PROJECT_DIR, "migrations", "alembic.ini")


def get_alembic_config():
    cfg = Config(ALEMBIC_INI)
    cfg.set_main_option("script_location", os.path.join(PROJECT_DIR, "migrations"))
    return cfg


def cmd_init(args):
    """Create the database and run all migrations."""
    cfg = get_alembic_config()
    command.upgrade(cfg, "head")
    print("Database initialized.")


def cmd_migrate(args):
    """Generate a new migration from model changes."""
    cfg = get_alembic_config()
    msg = args.message or "auto migration"
    command.revision(cfg, autogenerate=True, message=msg)
    print(f"Migration created: {msg}")


def cmd_upgrade(args):
    """Apply pending migrations."""
    cfg = get_alembic_config()
    rev = args.revision or "head"
    command.upgrade(cfg, rev)
    print(f"Upgraded to: {rev}")


def cmd_downgrade(args):
    """Rollback one migration."""
    cfg = get_alembic_config()
    rev = args.revision or "-1"
    command.downgrade(cfg, rev)
    print(f"Downgraded to: {rev}")


def cmd_current(args):
    cfg = get_alembic_config()
    command.current(cfg, verbose=True)


def cmd_history(args):
    cfg = get_alembic_config()
    command.history(cfg, verbose=True)


def main():
    parser = argparse.ArgumentParser(description="Addressbook schema management")
    sub = parser.add_subparsers(dest="command", help="available commands")
    sub.required = True

    sub.add_parser("init", help="Initialize database with current schema")

    migrate_p = sub.add_parser("migrate", help="Auto-generate migration")
    migrate_p.add_argument("-m", "--message", help="Migration description")

    upgrade_p = sub.add_parser("upgrade", help="Apply migrations")
    upgrade_p.add_argument("revision", nargs="?", default="head")

    downgrade_p = sub.add_parser("downgrade", help="Rollback migration")
    downgrade_p.add_argument("revision", nargs="?", default="-1")

    sub.add_parser("current", help="Show current revision")
    sub.add_parser("history", help="Show migration history")

    args = parser.parse_args()

    handlers = {
        "init": cmd_init,
        "migrate": cmd_migrate,
        "upgrade": cmd_upgrade,
        "downgrade": cmd_downgrade,
        "current": cmd_current,
        "history": cmd_history,
    }
    handlers[args.command](args)


if __name__ == "__main__":
    main()
