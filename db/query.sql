create table if not exists users (
    id integer primary key autoincrement,
    login text not null unique,
    username text not null,
    password text not null,
    createdAt timestamp default current_timestamp
);

create table if not exists messages (
    id integer primary key autoincrement,
    chatId integer not null,
    authorId integer not null,
    body text not null,
    createdAt timestamp default current_timestamp,

    foreign key(authorId) references users(id),
    foreign key(chatId) references chats(id)
);

create table if not exists chats (
    id integer primary key autoincrement,
    name text not null,
    lastMessageId integer,
    createdAt timestamp default current_timestamp,

    foreign key(lastMessageId) references messages(id)
);

create table if not exists chatParticipants (
    id integer primary key autoincrement,
    chatId integer not null,
    userId integer not null,
    joinedAt timestamp default current_timestamp,

    foreign key(chatId) references chats(id),
    foreign key(userId) references users(id)
);

create table if not exists sessions (
    userId integer not null unique,
    token text not null unique,
    createdAt timestamp default current_timestamp,

    foreign key(userId) references users(id)
);
