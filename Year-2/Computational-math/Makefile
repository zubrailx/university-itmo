.PHONY: run all clean


run:
	python3 src/main.py

test:
	python3 src/test.py

clean:
	find . -type d -name "*__pycache__" -exec rm -rf {} +

pack:
	zip computational-math.zip -r *

