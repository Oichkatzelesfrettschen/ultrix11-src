"""Simple code analysis to extract locking function names."""
import re
from pathlib import Path


def find_locking_functions(directory: str):
    """Search for functions with 'lock' in the name."""
    pattern = re.compile(r"\n([a-zA-Z_][a-zA-Z0-9_]*)\s*\(")
    for path in Path(directory).rglob('*.c'):
        text = path.read_text(errors='ignore')
        for match in pattern.finditer(text):
            func = match.group(1)
            if 'lock' in func.lower():
                print(f"{path}: {func}")


if __name__ == "__main__":
    find_locking_functions('sys')
