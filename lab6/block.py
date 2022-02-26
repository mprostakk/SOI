from dataclasses import dataclass

from fs_parameters import BLOCK_SIZE, NUMBER_OF_BYTES_INT
from helpers import int_bytes, bytes_int


@dataclass
class Block:
    data: bytes
    next_block: int

    def __bytes__(self) -> bytes:
        assert len(self.data) == BLOCK_SIZE
        return self.data + int_bytes(self.next_block)

    @classmethod
    def create_from_bytes(cls, b: bytes):
        data = b[0:BLOCK_SIZE]
        return cls(data=data, next_block=bytes_int(b[BLOCK_SIZE:BLOCK_SIZE+NUMBER_OF_BYTES_INT]))

    @classmethod
    def number_of_bytes(cls) -> int:
        return BLOCK_SIZE + NUMBER_OF_BYTES_INT
