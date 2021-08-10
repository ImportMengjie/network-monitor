CREATE TABLE if not exists running_log (
	"log_type" int4 NOT NULL,
	"log_datetime" timestamptz(6) NOT NULL,
	"log_content" varchar(1000 char) NOT NULL
);

CREATE TABLE if not exists running_state (
	"deviceid" varchar(10 char) NOT NULL,
	"create_datetime" timestamptz(6) NOT NULL,
	"parse_datetime" timestamptz(6) NOT NULL,
	"state" varchar(10 char) NOT NULL,
	"content" varchar(1000 char) NOT NULL
);

