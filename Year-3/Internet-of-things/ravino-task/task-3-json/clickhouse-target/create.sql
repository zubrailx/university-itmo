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
  payload String
) Engine = Kafka
SETTINGS
  kafka_broker_list = 'kafka:29092',
  kafka_topic_list = 'test.users',
  kafka_group_name = 'users_consumer_group1',
  kafka_skip_broken_messages = 10000,
  kafka_format = 'JSONAsString';

create materialized view test_users_queue_mv to test_users as
select 
  JSONExtractInt(payload, '_id') AS id,
  JSONExtractString(payload, 'firstname') AS firstname,
  JSONExtractString(payload, 'lastname') AS lastname,
  JSONExtractInt(payload, 'age') AS age,
  JSONExtractString(payload, 'email') AS email,
  JSONExtractString(payload, 'sex') AS sex
from (select JSONExtractString(payload, 'payload') as payload from test_users_queue);
