drop table if exists test_users;
drop table if exists test_users_queue;
drop view if exists test_users_queue_mv;

create table test_users (
  id Int32,
  firstname String,
  lastname String,
  age Int32,
  email String,
  sex String
) Engine = MergeTree
order by (id)
;

create table test_users_queue (
  _id Int32,
  firstname String,
  lastname String,
  age Int32,
  email String,
  sex String,
) Engine = Kafka
SETTINGS
  kafka_broker_list = 'kafka:29092',
  kafka_topic_list = 'test.users',
  kafka_group_name = 'users_consumer_group1',
  kafka_format = 'AvroConfluent';

create materialized view test_users_queue_mv to test_users as
select _id, firstname, lastname, age, email, sex from test_users_queue;
