import {  Gradient } from "@/components/UI"
import styles from "./Home.module.css"
import { IChat, IUser, MessageType } from "@/types"

import { useFetching } from "@/hooks/useFetch"
import UserService from "@/api/UserService"

import {
	ChatInfo,
	AddFriends,
	FriendsList,
	Settings,
	MessagePrompt,
	Chat
} from "@/components"
import { useEffect, useState } from "react"
import ChatService from "@/api/ChatService"

const messages: MessageType[] = [
	{
		text: "Hi",
		author: "Ivan"
	},
	{
		text: "How are u?",
		author: "Ivan"
	},
	{
		text: "Hello!",
		author: "me"
	},
	{
		text: "Ok",
		author: "Ivan"
	}
]

const HomePage = (): JSX.Element => {
	const [friends, setFriends] = useState<IChat[] | null>(null)

	const [fetchUsers, isLoading, error] = useFetching(async () => {
		const data = await ChatService.getAll()
		setFriends(data)
	})

	useEffect(() => {
		fetchUsers();
	}, []);

	if (error) {
		return <div>{error}</div>
	}

	if (isLoading) {
		return <div>Loading...</div>
	}

	return (
		<div className={styles.wrapper}>
			<Gradient />
			<div className={styles.container}>
				<AddFriends className={styles.add_friends} />
				<ChatInfo className={styles.info} />
				<FriendsList
					data={friends}
					className={styles.list}
				/>
				<Chat className={styles.messages} data={messages} />
				<Settings className={styles.settings} />
				<MessagePrompt className={styles.prompt} />
			</div>
		</div>
	)
}

export default HomePage
