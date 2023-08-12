#!/usr/bin/env python3

import io
import logging
import pathlib
import sys
from typing import List

import aocd
import pprint


class CaptureStdout(list):
    def __enter__(self) -> List[str]:
        self._stdout = sys.stdout
        sys.stdout = self._stringio = io.StringIO()
        return self

    def __exit__(self, *args) -> None:
        self.extend(self._stringio.getvalue().splitlines())
        sys.stdout = self._stdout


class AocHelper:
    def __init__(self, year: int, day: int) -> None:
        self._puzzle = aocd.models.Puzzle(
            year=year, day=day, user=aocd.models.User(token=self.sessionToken())
        )

    @staticmethod
    def _sessionToken() -> str:
        with CaptureStdout() as output:
            aocd.cookies.scrape_session_tokens()

        assert len(output) == 1
        token = output[0].split()[0]
        logging.info(f"Session token: {token}\n")
        return token

    def sessionToken(self, force_reload=False) -> str:
        if force_reload or not hasattr(self, "_token"):
            self._token = self._sessionToken()

        return self._token

    def dataInput(self) -> str:
        return self._puzzle.input_data

    def dataExamples(self) -> List[aocd.examples.Example]:
        return self._puzzle.examples


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-y", "--year", type=int, required=True, choices=range(2015, 2023)
    )
    parser.add_argument("-d", "--day", type=int, required=True, choices=range(1, 26))
    parser.add_argument("-r", "--repo-root", type=pathlib.Path, required=True)
    parser.add_argument("-s", "--src-dir", type=pathlib.Path, default="src")
    parser.add_argument("-l", "--lib-dir", type=pathlib.Path, default="src/lib")
    args = parser.parse_args()

    # clear argv for subsequent calls to aocd functions that use argparse
    sys.argv = [sys.argv[0]]

    logging.basicConfig(level=logging.INFO)
    logging.info("----- Setting up advent of code skeleton -----")
    for i, kwarg in enumerate(args._get_kwargs()):
        name, value = kwarg
        suffix = "\n" if (i == len(args._get_kwargs()) - 1) else ""
        logging.info(f"{name.capitalize()}: {value}{suffix}")

    logging.info(f"----- Searching for AOC session -----")
    aoc = AocHelper(args.year, args.day)

    logging.info(f"----- Found AOC year {args.year}, day {args.day} -----")
    logging.info(f"Examples:\n{pprint.pformat(aoc.dataExamples())}")
    logging.info(f"Input:\n{pprint.pformat(aoc.dataInput())}")
