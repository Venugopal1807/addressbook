"""
SQLAlchemy model for the contacts table.

This mirrors the schema used by the C++ app's DatabaseManager.
Used by Alembic for generating migration scripts.
"""

from sqlalchemy import Column, Integer, String, Date, CheckConstraint
from sqlalchemy.orm import declarative_base

Base = declarative_base()


class Contact(Base):
    __tablename__ = "contacts"

    id = Column(Integer, primary_key=True, autoincrement=True)
    name = Column(String(100), nullable=False)
    mobile = Column(String(20), nullable=False)
    email = Column(String(254), nullable=False)
    birthday = Column(String(10), nullable=False)  # stored as ISO date string, same as C++ side

    # basic sanity constraints
    __table_args__ = (
        CheckConstraint("length(name) >= 2", name="ck_name_min_length"),
        CheckConstraint("length(email) >= 5", name="ck_email_min_length"),
    )

    def __repr__(self):
        return f"<Contact(id={self.id}, name='{self.name}', email='{self.email}')>"
