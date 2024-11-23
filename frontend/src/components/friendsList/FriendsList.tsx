import cls from "@/utils/cls"
import styles from "./FriendsList.module.css"
import { FriendsItem } from "./friendsItem/FriendsItem"
import { IUser } from "@/types"

interface FriendsListProps {
	className?: string
	data: IUser[] | null
}

export const FriendsList = ({
	className = "",
	data
}: FriendsListProps): JSX.Element => (
	<ul className={cls(styles.list, className)}>
		{ data && data.map((item: IUser, index: number): JSX.Element => (
			<FriendsItem data={item} index={index+1} key={index} />
		))}
	</ul>
)
