import styles from "./Chat.module.css"

import { ChatMessages } from "./chatMessages/ChatMessages"
import { ChatTop } from "./chatTop/ChatTop"
import { ChatBottom } from "./chatBottom/ChatBottom"

import { MessageType } from "./types"

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

export const Chat = (): JSX.Element => (
	<div className={styles.chat}>
		<ChatTop />
		<ChatMessages data={Messages} />
		<ChatBottom />
	</div>
)
