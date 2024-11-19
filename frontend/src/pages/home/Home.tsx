import {  Gradient } from "@/components/UI"

import styles from "./Home.module.css"

import { FriendsList, Chat } from "@/components"

const HomePage = () => {
	return (
		<div className={styles.wrapper}>
			<Gradient />
			<div className={styles.container}>
				<FriendsList />
				<Chat />
			</div>
		</div>
	)
}

export default HomePage
