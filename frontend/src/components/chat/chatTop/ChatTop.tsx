import styles from "./ChatTop.module.css"

import { Typography } from "@/components/UI"

export const ChatTop = () => (
	<div className={styles.chat_top}>
		<Typography tag="h2" variant="title-4">
			Name
		</Typography>
		<Typography tag="span" variant="text_small">
			last seen
		</Typography>
	</div>
)
