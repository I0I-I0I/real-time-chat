import cls from "@/utils/cls"
import styles from "./AddFriend.module.css"
import { Search } from "@/components/UI/search/Search"

interface AddFriendsProps {
	className?: string
}

export const AddFriends = ({
	className = ""
}: AddFriendsProps) => {
	return (
		<div className={cls(styles.add_friend, className)}>
			<Search />
		</div>
	)
}
