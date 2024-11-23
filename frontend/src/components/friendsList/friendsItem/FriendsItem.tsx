import { Typography } from "@/components/UI"

import styles from "./FriendsItem.module.css"

import cls from "@/utils/cls"
import { ReactSVG } from "react-svg"
import { NavLink } from "react-router-dom"

import { IUser } from "@/types"

interface TypographyProps {
	index: number
	className?: string
	data: IUser
}

export const FriendsItem = ({
	index,
	className = "",
	data
}: TypographyProps) => (
	<li className={cls(styles.item, className)} aria-selected aria-label="Friend">
		<NavLink to="#" className={styles.button}>
			<ReactSVG
				className={styles.icon}
				src="/account.svg"
			/>
			<div className={styles.body}>
				<Typography tag="span" variant="text_tiny" className={styles.index}>{String(index)}</Typography>
				<Typography tag="h2" variant="title-4">{data.username}</Typography>
				<Typography tag="span" variant="text_tiny">{data.login}</Typography>
			</div>
		</NavLink>
	</li>
)
