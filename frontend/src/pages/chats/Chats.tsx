import {  Gradient } from "@/components/UI"
import styles from "./Chats.module.css"
import { IChat,  MessageType } from "@/types"

import { useFetching } from "@/hooks/useFetch"

import {
	ChatInfo,
	AddChat,
	ChatsList,
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

const ChatsPage = (): JSX.Element => {
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
				<AddChat className={styles.add_friends} />
				<ChatInfo className={styles.info} />
				<ChatsList
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

export default ChatsPage
