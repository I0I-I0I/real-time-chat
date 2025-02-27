create table if not exists users (
	id integer primary key autoincrement,
	login text not null unique,
	username text not null,
	password text not null,
	created_at timestamp default current_timestamp
);

create table if not exists messages (
	id integer primary key autoincrement,
	chat_id integer,
	author_id integer,
	body text not null,
	created_at timestamp default current_timestamp,
	foreign key(author_id) references users(id),
	foreign key(chat_id) references chats(id)
);

create table if not exists chats (
	id integer primary key autoincrement,
	name text not null,
	last_message_id integer,
	created_at timestamp default current_timestamp,
	foreign key(last_message_id) references messages(id)
);

create table if not exists chat_participants (
	id integer primary key autoincrement,
	chat_id integer not null,
	user_id integer not null,
	joined_at timestamp default current_timestamp,
	foreign key(chat_id) references chats(id),
	foreign key(user_id) references users(id)
);
