"""Initial contacts table

Revision ID: 001
Create Date: 2025-01-15
"""
from alembic import op
import sqlalchemy as sa

revision = '001'
down_revision = None
branch_labels = None
depends_on = None


def upgrade():
    op.create_table(
        'contacts',
        sa.Column('id', sa.Integer(), primary_key=True, autoincrement=True),
        sa.Column('name', sa.String(100), nullable=False),
        sa.Column('mobile', sa.String(20), nullable=False),
        sa.Column('email', sa.String(254), nullable=False),
        sa.Column('birthday', sa.String(10), nullable=False),
        sa.CheckConstraint('length(name) >= 2', name='ck_name_min_length'),
        sa.CheckConstraint('length(email) >= 5', name='ck_email_min_length'),
    )


def downgrade():
    op.drop_table('contacts')
