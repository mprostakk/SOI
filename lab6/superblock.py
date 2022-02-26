from dataclasses import dataclass

from fs_parameters import NUMBER_OF_BYTES_INT
from helpers import int_bytes, bytes_int


@dataclass
class SuperBlock:
    magic_number: int
    number_of_inodes_blocks: int
    number_of_inodes: int
    number_of_blocks: int

    def __bytes__(self) -> bytes:
        return int_bytes(self.magic_number) + int_bytes(self.number_of_inodes_blocks) + int_bytes(self.number_of_inodes) + int_bytes(self.number_of_blocks)

    @classmethod
    def create_from_bytes(cls, b: bytes):
        return cls(
            magic_number=bytes_int(b[0:4]),
            number_of_inodes_blocks=bytes_int(b[4:8]),
            number_of_inodes=bytes_int(b[8:12]),
            number_of_blocks=bytes_int(b[12:16])
        )

    @classmethod
    def number_of_bytes(cls) -> int:
        return 4 * NUMBER_OF_BYTES_INT

