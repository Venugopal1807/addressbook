"""Initial schema - create contacts table

Revision ID: 001
Revises: None
Create Date: 2026-03-11
"""

from typing import Sequence, Union
from alembic import op
import sqlalchemy as sa

# Revision identifiers
revision: str = "001"
down_revision: Union[str, None] = None
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    """Create the contacts table."""
    op.create_table(
        "contacts",
        sa.Column("id", sa.Integer(), primary_key=True, autoincrement=True),
        sa.Column("name", sa.String(100), nullable=False),
        sa.Column("mobile", sa.String(20), nullable=False),
        sa.Column("email", sa.String(254), nullable=False),
        sa.Column("birthday", sa.Date(), nullable=False),
    )


def downgrade() -> None:
    """Drop the contacts table."""
    op.drop_table("contacts")
