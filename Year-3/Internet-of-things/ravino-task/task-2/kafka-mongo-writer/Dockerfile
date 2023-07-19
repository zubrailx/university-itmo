FROM python:3.9

RUN mkdir -p /app
WORKDIR /app

COPY requirements.txt .
RUN python -m pip install -r requirements.txt

COPY app.py .

CMD ["faust", "-A", "app", "worker", "-l", "info"]
