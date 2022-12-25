#!/bin/bash

files=$(find . -not -path '*/.*' -name '*.py' -type f)

coverage run -m pytest --verbose
echo $files | xargs -t python3 -m coverage report
echo $files | xargs -t python3 -m pycodestyle --ignore=E501
echo $files | xargs -t python3 -m pylint
echo $files | xargs -t mypy --strict
