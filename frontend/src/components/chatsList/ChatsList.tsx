import cls from "@/utils/cls"
import styles from "./ChatsList.module.css"
import { ChatsItem } from "./chatsItem/ChatsItem"
import { IChat, IChatPost } from "@/types"
import ChatService from "@/api/ChatService"

interface ChatsListProps {
	className?: string
	data: IChat[] | null
}

export const ChatsList = ({
	className = "",
	data
}: ChatsListProps): JSX.Element => {
	const onAddChat = async (e: React.MouseEvent<HTMLButtonElement, MouseEvent>, data: IChatPost) => {
		e.preventDefault()
		const status = await ChatService.createOne({
			name: data.name,
		})
		if (!status) alert("Error on creating chat")
	}

	return (
		<ul className={cls(styles.list, className)}>
			{ data && data.map((item: IChat, index: number): JSX.Element => (
				<ChatsItem data={item} index={index+1} key={index} />
			))}
			<ChatsItem onAddChat={onAddChat} data={"Add chat"} index={9} key={0} />
		</ul>
	)
}
