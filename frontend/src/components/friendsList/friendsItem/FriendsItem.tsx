import { Typography } from "@/components/UI"

import styles from "./FriendsItem.module.css"
import { FriendData } from "../types"

import cls from "@/utils/cls"

interface TypographyProps {
	index: number
	className?: string
	data: FriendData
}

export const FriendsItem = ({
	index,
	className = "",
	data
}: TypographyProps) => (
	<li className={cls(styles.item, className)} aria-selected aria-label="Friend">
		<button className={styles.button}>
			<Typography tag="span" variant="text_tiny" className={styles.index}>{String(index)}</Typography>
			<Typography tag="h2" variant="title-4">{data.name}</Typography>
			<Typography tag="p" variant="text_small">{data.lastMessage}</Typography>
		</button>
	</li>
)