import cls from "@/utils/cls"
import styles from "./AddChat.module.css"
import { Search } from "@/components/UI/search/Search"

interface AddChatProps {
	className?: string
}

export const AddChat = ({
	className = ""
}: AddChatProps) => {
	return (
		<div className={cls(styles.add_chat, className)}>
			<Search />
		</div>
	)
}
