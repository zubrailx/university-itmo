.PHONY: all export export_psql export_ddls

STUDENT_ID=s312563
STUDENT_PATH=isbd/term
HOSTNAME=se.ifmo.ru
HOSTPORT=2222
SCP=scp

SQL_DDL_DIR=ddl
SQL_DDL_NAMES=$(patsubst $(SQL_DDL_DIR)/%, %, $(wildcard $(SQL_DDL_DIR)/*.sql))
SQL_DDL_TARGET=.

PSQL_DIR=scripts
PSQL_NAMES=$(patsubst $(PSQL_DIR)/%, %, $(wildcard $(PSQL_DIR)/*.sh))
PSQL_TARGET=.

all: export

# Copy sql and sh scripts
export: export_ddls export_psql

export_psql:
	@$(SCP) -P $(HOSTPORT) $(patsubst %, $(PSQL_DIR)/%, $(PSQL_NAMES)) \
		$(STUDENT_ID)@$(HOSTNAME):/home/studs/$(STUDENT_ID)/$(STUDENT_PATH)/$(SQL_DDL_TARGET)

export_ddls:
	@$(SCP) -P $(HOSTPORT) $(patsubst %, $(SQL_DDL_DIR)/%, $(SQL_DDL_NAMES)) \
		$(STUDENT_ID)@$(HOSTNAME):/home/studs/$(STUDENT_ID)/$(STUDENT_PATH)/$(PSQL_TARGET)
