"""
SQLAlchemy ORM model for the Addressbook database schema.

This module defines the contacts table using SQLAlchemy's declarative
mapping. It mirrors the schema used by the Qt/C++ application and serves
as the single source of truth for database migrations managed by Alembic.

Usage:
    Used by Alembic for auto-generating and applying schema migrations.
    The C++ application uses QSqlDatabase for runtime access.
"""

from sqlalchemy import Column, Integer, String, Date, create_engine
from sqlalchemy.orm import declarative_base

Base = declarative_base()


class Contact(Base):
    """
    Represents a contact in the addressbook.

    Attributes:
        id: Auto-incrementing primary key.
        name: Full name of the contact (required, max 100 chars).
        mobile: Mobile phone number (required, max 20 chars).
        email: Email address (required, max 254 chars).
        birthday: Date of birth (required, stored as ISO 8601 date).
    """

    __tablename__ = "contacts"

    id = Column(Integer, primary_key=True, autoincrement=True)
    name = Column(String(100), nullable=False)
    mobile = Column(String(20), nullable=False)
    email = Column(String(254), nullable=False)
    birthday = Column(Date, nullable=False)

    def __repr__(self) -> str:
        return (
            f"<Contact(id={self.id}, name='{self.name}', "
            f"email='{self.email}')>"
        )
