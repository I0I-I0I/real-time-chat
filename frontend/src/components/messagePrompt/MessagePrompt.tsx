import { useId } from "react"
import styles from "./MessagePrompt.module.css"

import { Button, Input, Label } from "@/components/UI"
import cls from "@/utils/cls"

interface MessagePromptProps {
	className?: string
}

export const MessagePrompt = ({
	className = ""
}: MessagePromptProps): JSX.Element => {
	const id = useId()

	const OnChooseFileClick = (e: React.MouseEvent) => {
		e.preventDefault()
		alert("In development")
	}

	return (
		<div className={cls(styles.message_prompt, className)}>
			<form action="">
				<Label htmlFor={id} variant="file" onClick={OnChooseFileClick}>
					<span>choose file</span>
					<Input id={id} type="file" variant="file" />
				</Label>
				<Input type="text" variant="message_input" placeholder="Message..." />
				<Button type="submit" variant="send">Send</Button>
			</form>
		</div>
	)
}