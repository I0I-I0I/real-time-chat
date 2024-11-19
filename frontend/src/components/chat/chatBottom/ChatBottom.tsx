import styles from "./ChatBottom.module.css"

import { Button, Input } from "@/components/UI"

export const ChatBottom = (): JSX.Element => (
	<div className={styles.chat_bottom}>
		<form action="">
			<Input type="file" />
			<Input type="text" placeholder="Message..." />
			<Button type="submit">Send</Button>
		</form>
	</div>
)
