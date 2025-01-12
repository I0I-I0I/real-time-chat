import { Button, Typography } from "@/components/UI"

import styles from "./ChatsItem.module.css"

import cls from "@/utils/cls"
import { ReactSVG } from "react-svg"

import { IChat, IChatPost } from "@/types"

type DataType = IChat | string

interface ChatsItemProps {
	index: number
	className?: string
	data: DataType
	onAddChat?: ((e: React.MouseEvent<HTMLButtonElement, MouseEvent>, data: IChatPost) => void) | null
}

export const ChatsItem = ({
	index,
	className = "",
	data,
	onAddChat = null
}: ChatsItemProps) => (
	<li className={cls(styles.item, className)} aria-selected aria-label="Chat">
		<Button
			className={styles.button}
			onClick={(e: React.MouseEvent<HTMLButtonElement, MouseEvent>): void => {
				if (onAddChat !== null) {
					console.log("TEST")
					onAddChat(e, { name: "Test", lastMessage: "Message" })
				}
			}
		}>
		<>
			<ReactSVG
				className={styles.icon}
				src={ typeof data === "string" ? "/add_chat.svg" : "/account.svg"}
			/>
			{ typeof data === "string" ? (
				<div className={styles.body} >
					<Typography tag="h2" variant="title-4">{data}</Typography>
				</div>
			) : (
				<div className={styles.body}>
					<Typography tag="span" variant="text_tiny" className={styles.index}>{String(index)}</Typography>
					<Typography tag="h2" variant="title-4">{data.name}</Typography>
					<Typography tag="span" variant="text_tiny">{data?.lastMessage}</Typography>
				</div>
			)}
		</>
		</Button>
	</li>
)
