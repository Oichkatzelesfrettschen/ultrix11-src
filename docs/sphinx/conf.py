"""Sphinx configuration for the Ultrix-11 documentation."""

from pathlib import Path
import sys

# Add project root to sys.path for autodoc when needed
sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "src"))

project = "Ultrix-11"
author = "Ultrix Community"
release = "0.1"

extensions = [
    "sphinx.ext.autodoc",
    "sphinx.ext.napoleon",
    "breathe",
]

# Breathe configuration links Sphinx to the Doxygen XML output
breathe_projects = {
    "Ultrix-11": Path(__file__).resolve().parents[2] / "docs" / "doxygen" / "xml"
}
breathe_default_project = "Ultrix-11"

templates_path = ["_templates"]
exclude_patterns = []

# HTML output options
html_theme = "alabaster"
html_static_path = ["_static"]

def setup(app):
    """Hook to include project customizations."""
    app.add_css_file("custom.css")
