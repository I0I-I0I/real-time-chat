import styles from "./FriendsList.module.css"
import { FriendsItem } from "./friendsItem/FriendsItem"
import { FriendData } from "./types"

const Friends: FriendData[] = [
	{
		id: 1,
		name: "Ivan",
		lastMessage: "Ale"
	},
	{
		id: 1,
		name: "John",
		lastMessage: "Bye!"
	},
	{
		id: 1,
		name: "Alex",
		lastMessage: "How are you?"
	},
	{
		id: 1,
		name: "Sergey",
		lastMessage: "Ok"
	},
]

export const FriendsList = (): JSX.Element => (
	<ul className={styles.list}>
		{ Friends.map((item: FriendData , index: number): JSX.Element => (
			<FriendsItem data={item} index={index+1} />
		))}
	</ul>
)
