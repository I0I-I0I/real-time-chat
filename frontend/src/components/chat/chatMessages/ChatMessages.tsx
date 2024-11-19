import { Message } from "../message/Message"

import { MessageType } from "../types"

import styles from "./ChatMessages.module.css"

interface ChatMessagesProps {
	data: MessageType[]
}

export const ChatMessages = ({
	data
}: ChatMessagesProps) => {
	return (
		<ul className={styles.messages}>
			{ data.map((item: MessageType, index: number): JSX.Element => (
				<Message key={index} variant={item.author === "me" ? "right" : "left"}>{item.text}</Message>
			)) }
		</ul>
	)
}
