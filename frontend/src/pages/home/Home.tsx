import {  Gradient } from "@/components/UI"

import styles from "./Home.module.css"

import { MessageType } from "@/types"

import {
	ChatInfo,
	AddFriends,
	FriendsList,
	Settings,
	MessagePrompt,
	Chat
} from "@/components"

const Messages: MessageType[] = [
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

const HomePage = () => {
	return (
		<div className={styles.wrapper}>
			<Gradient />
			<div className={styles.container}>
				<AddFriends className={styles.add_friends} />
				<ChatInfo className={styles.info} />
				<FriendsList className={styles.list} />
				<Chat className={styles.messages} data={Messages} />
				<Settings className={styles.settings} />
				<MessagePrompt className={styles.prompt} />
			</div>
		</div>
	)
}

export default HomePage
