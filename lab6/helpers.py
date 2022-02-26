import fs_parameters


def int_bytes(value: int) -> bytes:
    return value.to_bytes(fs_parameters.NUMBER_OF_BYTES_INT, byteorder='big', signed=False)


def bytes_int(value: bytes) -> int:
    return int.from_bytes(value, byteorder='big', signed=False)
