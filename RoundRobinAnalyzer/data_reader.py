def is_valid_file(file_path) -> bool:
    """
    Checks if the file is a valid Round Robin data file.

    Args:
        file_path (str): The path to the file.

    Returns:
        bool: True if the file is a valid Round Robin data file, False otherwise.
    """

    first_word = "<START OF FILE>"
    last_word = "<END OF FILE>"
    with open(file_path, 'r') as file:
        lines = file.readlines()

    first_line = lines[0].strip()
    last_line = lines[-1].strip()

    if first_word in first_line and last_word in last_line:
        return True
    else:
        return False
