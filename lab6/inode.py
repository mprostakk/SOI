from dataclasses import dataclass

from fs_parameters import NUMBER_OF_BYTES_INT
from helpers import int_bytes, bytes_int


@dataclass
class INode:
    number: int
    type: int
    direct_pointers: list[int]
    indirect_pointer: int

    def __bytes__(self) -> bytes:
        b = int_bytes(self.number) + int_bytes(self.type)
        for direct_pointer in self.direct_pointers:
            b = b + int_bytes(direct_pointer)
        b = b + int_bytes(self.indirect_pointer)
        return b

    @classmethod
    def create_from_bytes(cls, b: bytes):
        direct_pointers = []
        index = 8
        for x in range(10):
            p = b[index:index+NUMBER_OF_BYTES_INT]
            index += NUMBER_OF_BYTES_INT
            direct_pointers.append(bytes_int(p))

        return cls(number=bytes_int(b[0:4]), type=bytes_int(b[4:8]), direct_pointers=direct_pointers, indirect_pointer=bytes_int(b[index:index+NUMBER_OF_BYTES_INT]))

    @classmethod
    def number_of_bytes(cls) -> int:
        return 3 * NUMBER_OF_BYTES_INT + 10 * NUMBER_OF_BYTES_INT

    def is_empty(self):
        return self.type == 0

    def is_file(self):
        return self.type == 1

    def is_directory(self):
        return self.type == 2
