import cls from "@/utils/cls"
import styles from "./FriendsList.module.css"
import { FriendsItem } from "./friendsItem/FriendsItem"
import { IChat, IChatPost, IUser } from "@/types"
import ChatService from "@/api/ChatService"

interface FriendsListProps {
	className?: string
	data: IChat[] | null
}

export const FriendsList = ({
	className = "",
	data
}: FriendsListProps): JSX.Element => {
	const onAddChat = async (e: React.MouseEvent<HTMLButtonElement, MouseEvent>, data: IChatPost) => {
		e.preventDefault()
		const status = await ChatService.createOne({
			name: data.name,
		})
		if (!status) alert("Error on creating chat")
	}

	return (
		<ul className={cls(styles.list, className)}>
			{ data && data.map((item: IUser, index: number): JSX.Element => (
				<FriendsItem data={item} index={index+1} key={index} />
			))}
			<FriendsItem onAddChat={onAddChat} data={"Add chat"} index={9} key={0} />
		</ul>
	)
}
