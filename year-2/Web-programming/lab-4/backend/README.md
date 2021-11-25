# Лабораторная работа №4. Вариант 30613

### Notes

* Uses custom standalone.xml (lab is executed on ~~WIldfly 24.0.1 (CORS is not enabled on Wildfly)~~ Tomcat 9.0.52), 
to enable spring logger. 
* Username and password are read from bin/credentials.txt in application server directory.
* There are both ordinary users and admins who have different rights of use.
* Unauthenticated users have access only to authenticate, register and token change.
* There are both access token and refresh tokens.
* When access token is expired, need to auth again or refresh tokens.


## API

### User

| Type |        Command        | Format | Required Roles |                                           Body                                           |
| ---- | --------------------- | ------ | -------------- | ---------------------------------------------------------------------------------------- |
| POST | /api/v1/user/register | JSON   |                | <pre>{<br>"username": STRING,<br>"password": STRING, <br> <"roles": [STRING]><br>}</pre> |
| POST | /api/v1/user/auth     | JSON   |                | <pre>{<br>"username": STRING,<br>"password": STRING <br>}</pre>                          |
| POST | /api/v1/user/refresh  | JSON   |                | <pre>{<br>"refreshToken": STRING <br>}</pre>                                             |
| GET  | /api/v1/user/all      |        | ROLE_ADMIN     |                                                                                          |


### Point

| Type |             Command              | Format |             Required Roles             |                                            Body                                            |
| ---- | -------------------------------- | ------ | -------------------------------------- | ------------------------------------------------------------------------------------------ |
| GET  | /api/v1/point/get?{pointId=LONG} |        | ROLE_USER                              |                                                                                            |
| GET  | /api/v1/point/get/{userId}       |        | ROLE_USER                              |                                                                                            |
| POST | /api/v1/point/save               | JSON   | ROLE_USER                              | <pre>{<br>"coordinateX": DOUBLE,<br>"coordinateY": DOUBLE <br>"radius": DOUBLE <br>}</pre> |
| POST | /api/v1/point/save/{userId}      | JSON   | ROLE_ADMIN                             | <pre>{<br>"coordinateX": DOUBLE,<br>"coordinateY": DOUBLE <br>"radius": DOUBLE <br>}</pre> |
| POST | /api/v1/point/remove             | JSON   | ROLE_USER                              | <pre>LONG</pre>                                                                            |
| POST | /api/v1/point/remove/{userId}    |        | ROLE_ADMIN                             |                                                                                            |
| POST | /api/v1/point/remove_many        | JSON   | ROLE_USER - owned,<br>ROLE_ADMIN - all | <pre>{<br>"pointIds":[LONG]<br>}</pre>                                                     |
